#define _USE_MATH_DEFINES
#include "PhotonMap.h"


PhotonMap::PhotonMap(int nMaxPhotons)
{
	mvnStoredPhotons = 0;
	mvdPrevScale = 1;
	mvnMaxPhotons = nMaxPhotons;

	mvaPhotons = (Photon*)malloc( sizeof ( Photon ) * (mvnMaxPhotons + 1 ) );
	//mvaPhotons = new Photon[mvnMaxPhotons + 1];

	if(mvaPhotons == NULL)
	{
		fprintf(stderr,"Not enough memory to initialize photon map \n");
		exit(-1);
	}

	mvafBBoxMin[0] = mvafBBoxMin[1] = mvafBBoxMin[2] = 1e8f;
	mvafBBoxMax[0] = mvafBBoxMax[1] = mvafBBoxMax[2] = -1e8f;


	//********************************
	//Sample hemisphere and store in arrays
	//********************************
	for (int i = 0; i<256; i++)
	{
		double angle = double(i)*(1.0/256.0)*M_PI;
		mvafCosTheta[i]	= cos( angle );
		mvafSinTheta[i]	= sin( angle );
		mvafCosPhi[i]	= cos ( 2.0*angle );
		mvafSinPhi[i]	= sin ( 2.0*angle );
	}
}
PhotonMap::~PhotonMap()
{
	free(mvaPhotons);
}


//********************************
// Sets photon direction based in theta and phi indices
//********************************
void PhotonMap::photon_dir(float * pfDir, const Photon * pPhotons) const
{
	pfDir[0] = mvafSinTheta[pPhotons->ucTheta]*mvafCosPhi[pPhotons->ucPhi];
	pfDir[1] = mvafSinTheta[pPhotons->ucTheta]*mvafSinPhi[pPhotons->ucPhi];
	pfDir[2] = mvafCosTheta[pPhotons->ucTheta];
}

//********************************
// Locates the nearest photons given a NearestPhoton struct
//********************************
void PhotonMap::locate_photons(NearestPhotons * const pNearestPhotons, const int nIndex) const
{
	const Photon *pPhoton = &mvaPhotons[nIndex];
	float fDist1;

	if(nIndex < mvnHalfStoredPhotons)
	{
		//Distance to photon in the nPlane
		fDist1 = pNearestPhotons->afPos[pPhoton->nPlane] - pPhoton->afPos[pPhoton->nPlane];

		if(fDist1 > 0.0) // if fdist1 is positive, search right
		{
			locate_photons(pNearestPhotons,2*nIndex+1);
			if( fDist1*fDist1 < pNearestPhotons->pDist2[0] )
				locate_photons(pNearestPhotons,2*nIndex);
		}
		else //fdist1 is negative, search left
		{
			locate_photons(pNearestPhotons,2*nIndex);
			if( fDist1 * fDist1 < pNearestPhotons->pDist2[0] )
				locate_photons(pNearestPhotons,2*nIndex+1);
		}
	}

	//Compute squared distance between current photon and pNearestPhotons->afPos

	fDist1 = pPhoton->afPos[0] - pNearestPhotons->afPos[0];
	float fDist2 = fDist1*fDist1;
	fDist1 = pPhoton->afPos[1] - pNearestPhotons->afPos[1];
	fDist2 += fDist1*fDist1;
	fDist1 = pPhoton->afPos[2] - pNearestPhotons->afPos[2];
	fDist2 += fDist1*fDist1;


	if (fDist2 < pNearestPhotons->pDist2[0])
	{
		//Found a candite for nearest photon, instert in list	


		if(pNearestPhotons->nFound < pNearestPhotons->nMax)
		{ 
			//Heap is not full; use array
			pNearestPhotons->nFound++;
			pNearestPhotons->pDist2[pNearestPhotons->nFound] = fDist2;
			pNearestPhotons->ppPhotonIndex[pNearestPhotons->nFound] = pPhoton;
		}
		else
		{
			int nj,nParent;

			if (pNearestPhotons->nGot_heap == 0)
			{
				//Build heap
				float fTmpDist2;
				const Photon * pTmpPhoton;

				int nHalfFound = pNearestPhotons->nFound>>1; //Divide by 2
				for (int k = nHalfFound; k>=1; k--)
				{
					nParent = k;
					pTmpPhoton = pNearestPhotons->ppPhotonIndex[k];
					fTmpDist2 = pNearestPhotons->pDist2[k];
					while (nParent <= nHalfFound)
					{
						//nj = nParent + nParent = 2*nParent (but faster, not tmp object allocation)
						nj = nParent;
						nj += nParent;

						if(nj<pNearestPhotons->nFound && pNearestPhotons->pDist2[nj]<pNearestPhotons->pDist2[nj+1])
							nj++;
						if(fTmpDist2 >= pNearestPhotons->pDist2[nj])
							break;
						pNearestPhotons->pDist2[nParent] = pNearestPhotons->pDist2[nj];
						pNearestPhotons->ppPhotonIndex[nParent] = pNearestPhotons->ppPhotonIndex[nj];
						nParent = nj;
					}
					pNearestPhotons->pDist2[nParent] = fTmpDist2;
					pNearestPhotons->ppPhotonIndex[nParent] = pTmpPhoton;
				}
				pNearestPhotons->nGot_heap = 1;
			}

			//Insert new photon into max heap
			//Delete largest element, insert new, and reorder the heap

			nParent = 1;
			nj = 2;

			while ( nj <= pNearestPhotons->nFound)
			{
				if ( nj < pNearestPhotons->nFound && pNearestPhotons->pDist2[nj] < pNearestPhotons->pDist2[nj+1])
					nj++;
				if ( fDist2 > pNearestPhotons->pDist2[nj] )
					break;
				pNearestPhotons->pDist2[nParent] = pNearestPhotons->pDist2[nj];
				pNearestPhotons->ppPhotonIndex[nParent] = pNearestPhotons->ppPhotonIndex[nj];
				nParent = nj;
				nj += nj;
			}
			pNearestPhotons->ppPhotonIndex[nParent] = pPhoton;
			pNearestPhotons->pDist2[nParent] = fDist2;

			pNearestPhotons->pDist2[0] = pNearestPhotons->pDist2[1];
		}
	}
}

//********************************
//Stores a photon in the 1D photon array and updates the bounding box
//********************************
void PhotonMap::store(
	const float afPower[3], 
	const float afPos[3], 
	const float afDir[3])
{

	//Photon map is full
	if(mvnStoredPhotons > mvnMaxPhotons)
		return;

	mvnStoredPhotons++;

	//node points to last element in Photons array
	Photon *const node = &mvaPhotons[mvnStoredPhotons];

	//Update the bounding box and assign position and direction to node
	for(int i = 0; i<3; i++)
	{
		node->afPos[i] = afPos[i];

		if( node->afPos[i]  < mvafBBoxMin[i] )
			mvafBBoxMin[i] = node->afPos[i];
		if( node->afPos[i] > mvafBBoxMax[i] )
			mvafBBoxMax[i] = node->afPos[i];

		node->pafPower[i] = afPower[i];
	}

	//********************************
	//Set the direction indeces of the node
	//********************************

	//inTheta = index to theta array
	int inTheta = int( acos( afDir[2] )*( 256.0/M_PI) ); 
	node->ucTheta = inTheta > 255 ? 255 : (unsigned char)inTheta; //Clamp

	//inPhi = index to phi array
	int inPhi = int( atan2(afDir[1],afDir[0]*(256.0f/(2.0f*(float)M_PI)) ) );
	node->ucPhi = inPhi > 255 ? 255 : (unsigned char)inPhi; //Clamp

}

//********************************
// Scales all photons shot from one lightsource with fScale = 1 / #NUMBER_OF_PHOTONS
//********************************
void PhotonMap::scale_photon_power(const float fScale)
{
	for (int i = mvdPrevScale; i < mvnStoredPhotons; i++)
	{
		mvaPhotons[i].pafPower[0] *= fScale;
		mvaPhotons[i].pafPower[1] *= fScale;
		mvaPhotons[i].pafPower[2] *= fScale;
	}

	//Guarantee's that we don't scale these photons again
	mvdPrevScale = mvnStoredPhotons;
}

//********************************
//Creates a left-balanced kd-tree
//********************************
void PhotonMap::balance()
{
	//Temporary arrays to hold pointer to pointers in the photons array
	//We store pointers to pointers so we dont need to swap around the actual
	//nodes
	Photon	**ppa1 = (Photon **)malloc(sizeof(Photon*)*(mvnStoredPhotons + 1));
	Photon	**ppa2 = (Photon **)malloc(sizeof(Photon*)*(mvnStoredPhotons + 1));


	//Fill ppa2 with pointers to the pointers in the photons array
	for(int i = 0; i < mvnStoredPhotons; i++)
	{
		ppa2[i] = &mvaPhotons[i];
	}


}


//Swap method used in median_split()
#define swap(ph,a,b) {Photon *ph2 = ph[a]; ph[a] = ph[b]; ph[b] = ph2; }
//********************************
// Performs a median split based on nAxis around nMedian
// Photons below the median will be placed to the left of median
// Photon above the median will be placed to the right of the median
//********************************
void PhotonMap::median_split(
	Photon ** ppPhoton, 
	const int nStart, 
	const int nEnd, 
	const int nMEdian, 
	const int nAxis)
{
	int nLeft = nStart;
	int nRight = nEnd;

	while ( nRight > nLeft )
	{
		const float fV = ppPhoton[nRight]->afPos[nAxis];
		int ni = nLeft - 1;
		int nj = nRight;

		for(;;)
		{
			while ( ppPhoton[++ni]->afPos[nAxis] < fV)
				;
			while ( ppPhoton[--nj]->afPos[nAxis] > fV && nj > nLeft)
				;
			if( ni >= nj)
				break;
			swap(ppPhoton,ni,nj);
		}
	}
}


//********************************
// Recursive functions that will produce a photon array that is sorted 
// based on the median of each segment, Where the median is based on
// the slitting axis for the current segment.
//********************************
void PhotonMap::balance_segment(
	Photon ** ppaBalancedPhotons, 
	Photon ** ppaOriginalPhotons, 
	const int nIndex, 
	const int nStart, 
	const int nEnd)
{
	//********************************
	// First compute the index of the median of the current segment
	// The following algorithm will choose the actual median
	// or floor(median)
	// array:	- 1 2 3 4 5 //nStart = 1, nEnd = storedPhotons = 5
	// indices:	0 1 2 3 4 5	
	// -> median = 4! i.e *(array + median) = 3;
	//********************************

	///THIS MIGHT BE WRONG
	int nMedian = 1;
	while( (4*nMedian) <= (nEnd - nStart + 1))
		nMedian += nMedian;

	if ( (3*nMedian) <= (nEnd - nStart + 1))
	{
		nMedian += nMedian;
		nMedian += nStart-1;
	}
	else
		nMedian = nEnd-nMedian+1;

	//********************************
	//Find axis to split, Split the axis with greatest distance
	//********************************

	int nAxis = ZAXIS;
	if ((mvafBBoxMax[0] - mvafBBoxMin[0]) > (mvafBBoxMax[1] - mvafBBoxMin[1]) &&
		(mvafBBoxMax[0] - mvafBBoxMin[0]) > (mvafBBoxMax[2] - mvafBBoxMin[2]))
		nAxis = XAXIS;
	else if ((mvafBBoxMax[1]-mvafBBoxMin[1]) > (mvafBBoxMax[2] - mvafBBoxMin[2]))
		nAxis = YAXIS;


	//********************************
	// Partition photon block around the median based on nAxis
	// Ex: if nAxis = XAXIS do median split on x coordinate
	//********************************
	median_split(ppaOriginalPhotons,nStart,nEnd,nMedian,nAxis);

	ppaBalancedPhotons[nIndex] = ppaOriginalPhotons[nMedian];
	ppaBalancedPhotons[nIndex]->nPlane = nAxis;

	//********************************
	//Recursively balance the left and right block
	//********************************

	if(nMedian > nStart) // Balance the left segment
	{
		if( nStart < nMedian - 1)
		{
			const float fTmp = mvafBBoxMax[nAxis];
			mvafBBoxMax[nAxis] = ppaBalancedPhotons[nIndex]->afPos[nAxis];
			balance_segment(ppaBalancedPhotons,ppaOriginalPhotons,2*nIndex,nStart,nMedian-1);
			mvafBBoxMax[nAxis] = fTmp;
		}
		else
		{
			ppaBalancedPhotons[2*nIndex] = ppaOriginalPhotons[nStart];
		}
	}

	if(nMedian < nEnd) //Balance right segment
	{
		if(nMedian + 1 < nEnd)
		{
			const float fTmp = mvafBBoxMin[nAxis];
			mvafBBoxMin[nAxis] = ppaBalancedPhotons[nIndex]->afPos[nAxis];
			balance_segment(ppaBalancedPhotons,ppaOriginalPhotons,2*nIndex+1,nMedian+1,nEnd);
			mvafBBoxMin[nAxis] = fTmp;
		}
		else
		{
			ppaBalancedPhotons[2*nIndex+1] = ppaOriginalPhotons[nEnd];
		}
	}
}