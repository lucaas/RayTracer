//----------------------------------------------------------------
//
// Photon map data structure
// Based on the implementation in "Realistic Image Synthesis Using Photon Mapping
//
//----------------------------------------------------------------


#ifndef PHOTON_MAP_H
#define PHOTON_MAP_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>



//********************************
// Axis constants
//********************************
const short XAXIS = 0;
const short YAXIS = 1;
const short ZAXIS = 2;


//********************************
//Photon struct for storing information of a single photon, 28 bytes
//********************************
struct Photon 
{
	float afPos[3];				//Photon position
	short nPlane;				//Splitting plane for kd-Tree
	unsigned char ucTheta, ucPhi;	//Index to sampled direction arrays
	float pafPower[3];				//Photon power
};

//********************************
//Structure used to locate nearest photons
//********************************
struct NearestPhotons
{
	int nMax;
	int nFound;
	int nGot_heap;
	float afPos[3];
	float *pDist2;
	const Photon **ppPhotonIndex; //ptr to ptr in photons array
};


class PhotonMap
{
public:
	PhotonMap (int nMaxPhotons);
	~PhotonMap();

	void store(const float afPower[3], const float afPos[3], const float afDir[3]);

	void scale_photon_power(const float fScale);

	void balance(void);

	void irradiance_estimate(
		float afIrrad[3], 
		const float afPos[3],
		const float afNormal[3],
		const float fMax_dist,
		const int nPhotons ) const;

	void locate_photons(NearestPhotons * const pNearestPhotons, const int nIndex) const;

	void photon_dir(float * pfDir, const Photon * pPhotons) const;

private:

	void balance_segment(
		Photon ** ppaBalancedPhotons,
		Photon ** ppaOriginalPhotons,
		const int nIndex,
		const int nStart,
		const int nEnd); 

	void median_split(
		Photon ** ppPhoton,
		const int nStart,
		const int nEnd,
		const int nMEdian,
		const int nAxis);


	//********************************
	//Private Members
	//********************************

	Photon * mvaPhotons;			//1D array to hold photons
	int mvnStoredPhotons;			//Number of photons stores in mvaPhotons
	int mvnHalfStoredPhotons;		//
	int mvnMaxPhotons;				//Maximum number of photons
	int mvdPrevScale;				//Index to the previously scaled photons

	//********************************
	//Samples direction on hemisphere
	//********************************
	float mvafCosTheta[256];			
	float mvafSinTheta[256];
	float mvafCosPhi[256];
	float mvafSinPhi[256];

	//********************************
	// Bounding box of the photons
	//********************************
	float mvafBBoxMin[3];
	float mvafBBoxMax[3];
};

#endif