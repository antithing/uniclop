#ifndef __OVLOCALMATCHERT_H
#define __OVLOCALMATCHERT_H

#include "ImageT.hpp"


namespace Openvis3d {

///Abstract Base Class Template for matching a pair of images.
/** 
* The LocalMatcherT class defines a basic interface for matching a pair of images.
* Subclasses can implement the interface to provide a variety of local image matchers.
*
* @author Abhijit Ogale
*/
template<typename T>
class LocalMatcherT
{
public:

	LocalMatcherT(); /**< Default constructor with no parameters */

	virtual ~LocalMatcherT(); /**< Destructor */

	/**
	* Method used for specifying the image pair to be matched.
	* @param i1 the first image
	* @param i2 the second image
	* @return true if successful (both images are of the same dimensions).
	*/
	virtual bool setImagePair(const ImageT<T> & i1, const ImageT<T> & i2) = 0;

	/**
	* Used for specifying any parameters required by the matcher.
	* @param nparams number of parameters which are being passed
	* @param params the values of the parameters
	* @return true if successful.
	*/
	virtual bool setParams(int nparams, double*params) = 0;

	/**
	* Used to retrieve results of matching the two images with a relative 2D translation. 
	* Note: Values of the result are between 0 to 1 always.
	* @param shiftx horizontal relative shift
	* @param shifty vertical relative shift
	* @return resulting single channel image if sucessful (values between 0 (no match) and 1 (match)).
	* @see getRawMatch(int shiftx, int shifty)
	*/
	virtual const ImageT<double> getMatch(int shiftx, int shifty=0) = 0;

	/**
	* Used to retrieve raw results of matching the two images with a relative 2D translation. 
	* Note: The range of values of the result and their interpretation depends on the particular matcher being used.
	* @param shiftx horizontal relative shift
	* @param shifty vertical relative shift
	* @return result image if sucessful, which has same number of channels as the input images.
	* @see getMatch(int shiftx, int shifty)
	*/
	virtual const ImageT<double> getRawMatch(int shiftx, int shifty=0) = 0;
};

template<typename T>
LocalMatcherT<T>::LocalMatcherT()
{
}

template<typename T>
LocalMatcherT<T>::~LocalMatcherT()
{
}

}
#endif //__OVLOCALMATCHERT_H
