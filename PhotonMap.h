//----------------------------------------------------------------
//
// Photon map data structure
// Based on the implementation in "Realistic Image Synthesis Using Photon Mapping
//
//----------------------------------------------------------------


#ifndef PHOTON_MAP_H
#define PHOTON_MAP_H



//-----------------------------------------------------------------------------
// photonmap.cc
// An example implementation of the photon map data structure
//
// Henrik Wann Jensen - February 2001
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Vector3.h"

//********************************
// Axis constants
//********************************
const short XAXIS = 0;
const short YAXIS = 1;
const short ZAXIS = 2;
/*  This is the photon
*  The power is not compressed so the
*  size is 28 bytes
*/
//**********************
typedef struct Photon {
	//**********************
	cbh::vec3f pos;                  // photon position
	short plane;                   // splitting plane for kd-tree
	unsigned char theta, phi;      // incoming direction
	cbh::vec3f power;                // photon power (uncompressed)
} Photon;


/* This structure is used only to locate the
* nearest photons
*/
//*******************************
typedef struct NearestPhotons {
	//*******************************
	int max;
	int found;
	int got_heap;
	cbh::vec3f pos;
	float *dist2;
	const Photon **index;
} NearestPhotons;


/* This is the Photon_map class
*/
//*****************
class Photon_map {
	//*****************
public:
	Photon_map( int max_phot );
	~Photon_map();


	void store(
		const cbh::vec3f power,         // photon power
		const cbh::vec3f pos,           // photon position
		const cbh::vec3f dir );         // photon direction

	void scale_photon_power(
		const float scale );          // 1/(number of emitted photons)

	void balance(void);             // balance the kd-tree (before use!)

	void irradiance_estimate(
		cbh::vec3f & irrad,               // returned irradiance
		const cbh::vec3f & pos,           // surface position
		const cbh::vec3f & normal,        // surface normal at pos
		const float & max_dist,         // max distance to look for photons
		const int & nphotons ) const;   // number of photons to use

	void locate_photons(
		NearestPhotons *const np,     // np is used to locate the photons
		const int index ) const;      // call with index = 1

	void photon_dir(
		cbh::vec3f  & rdir,                    // direction of photon (returned)
		const Photon *p ) const;       // the photon

	Photon *photons;
private:

	void balance_segment(
		Photon **pbal,
		Photon **porg,
		const int index,

		const int start,
		const int end );

	void median_split(
		Photon **p,
		const int start,
		const int end,
		const int median,
		const int axis );


	int stored_photons;
	int half_stored_photons;
	int max_photons;
	int prev_scale;

	float costheta[266];
	float sintheta[266];
	float cosphi[256];
	float sinphi[256];

	cbh::vec3f bbox_min;     // use bbox_min;
	cbh::vec3f bbox_max;     // use bbox_max;
};

#endif