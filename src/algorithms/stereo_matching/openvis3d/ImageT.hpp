#ifndef __OVIMAGET_H
#define __OVIMAGET_H

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "ImageAdapter.hpp"


namespace Openvis3d {


///Internal image class with overloaded math operators and utility functions.
/** 
* The ImageT class is a template class used internally by the library,
* although it can easily be used for your own purposes. Arithmetic, comparison, logical, etc.
* operators are overloaded to perform per-pixel operations using a simple syntax. Mathematical
* functions (e.g., sin, exp, pow, etc.) are also overloaded. Common image operations such as
* 2D convolution, linear and nonlinear filtering, and several standard Matlab-style functions
* have been provided. 
*
* @author Abhijit Ogale
*/
template<typename T>
class ImageT
{
public:

  //constructors and destructors
  ImageT(); //create empty image
  ImageT(int height, int width, int nColorChannels);  //create image of given dimensions
  ImageT(const ImageT<T>& srcImage, bool CopyData=true); //create image with same size as input image, with the option of copying data
  template<typename C> ImageT(const ImageT<C>& srcImage, bool CopyData=true); //create image with same size as an input image of a different type (i.e., int, float, etc.), with the option of copying data
  virtual ~ImageT();

  void getDimensions(int & height, int & width, int & nColorChannels) const; //get image size
  int getHeight() const;
  int getWidth() const;
  int getChannels() const;
  void resetDimensions(int height, int width, int nColorChannels = 1); //reset image dimensions and fill with zeros
  void reshape(int height, int width, int nColorChannels = 1);  //reshape image without changing number of pixels
  void normalizeIntensityRange(); //rescale image intensities to lie between 0 and 1


  //for debugging
  void print(void); /**< print image contents (only for debugging) */

  //special operators	
  inline T& operator() (int row, int column = 0, int channel = 0); //allows easy indexing into the image, e.g., im(i,j,k)
  inline T& operator() (int row, int column = 0, int channel = 0) const; //const version of above operator
  ImageT<T>& operator = (const ImageT<T> & rhsImage); //assignment operator (e.g., i1 = i2; )
  ImageT<T>& operator = (const T & rhs); //assignment operator with scalar rhs (e.g., i1 = 5.2; )
  template <typename C> ImageT<T>& operator = (const ImageT<C> & rhsImage); //convert from one template type to another (e.g., float to int)

  //special copying methods
  bool copyFromAdapter(const ImageAdapter & iadapter); //import values from ImageAdapter
  bool copyToAdapter(ImageAdapter & iadapter); //export values to ImageAdapter if it has same dimensions
  bool copyMasked(const ImageT<bool> & mask, const ImageT<T> & srcImage); //copy values from srcImage only for pixels where mask is set to true
  bool copyMasked(const ImageT<bool> & mask, const T & value); //set pixels = value only where mask is set to true
  bool copyChannel(ImageT<T> & input, int inputchannel, int outputchannel); //copies a certain input channel to a certain output channel
  //bool copyRegion(const T & value, int rowLo=-1, int rowHi=-1, int columnLo=-1, int columnHi=-1, int channelLo=-1, int channelHi=-1);
  //bool copyRegionEx(const T & value, int rowLo=-1, int rowHi=-1, int columnLo=-1, int columnHi=-1, int channelLo=-1, int channelHi=-1);	

  const ImageT<T> getSubImage(int rowLo=-1, int rowHi=-1, int columnLo=-1, int columnHi=-1, int channelLo=-1, int channelHi=-1); //copy and return rectangular image block

  //arithmetic operators 
  ImageT<T> & operator += (const ImageT<T> & rhs);	/**< e.g., i1 += i2;*/
  ImageT<T> & operator += (const T & rhs);				/**< e.g., i1 += 3.2;*/
  ImageT<T> & operator -= (const ImageT<T> & rhs);	/**< e.g., i1 -= i2;*/
  ImageT<T> & operator -= (const T & rhs);				/**< e.g., i1 -= 5.5;*/
  ImageT<T> & operator *= (const ImageT<T> & rhs);	/**< e.g., i1 *= i2;*/
  ImageT<T> & operator *= (const T & rhs);				/**< e.g., i1 *= 2;*/
  ImageT<T> & operator /= (const ImageT<T> & rhs);	/**< e.g., i1 /= i2;*/
  ImageT<T> & operator /= (const T & rhs);				/**< e.g., i1 /= 10;*/

  ImageT<T> & operator ++ (); 							/**< e.g., ++i1;*/
  ImageT<T> & operator -- ();							/**< e.g., --i1;*/
  const ImageT<T> operator ++ (int); 					/**< e.g., i1++;*/
  const ImageT<T> operator -- (int);					/**< e.g., i1--;*/

  const ImageT<T> operator - (); 						/**< e.g., i1 = -i2;*/
  const ImageT<T> operator + ();						/**< e.g., i1 = +i2;*/

  template<typename C> friend const ImageT<C> operator + (const ImageT<C> & i1, const ImageT<C> & i2);  /**< e.g., i1 = i2+i3; */
  template<typename C> friend const ImageT<C> operator + (const double i1, const ImageT<C> & i2);			/**< e.g., i1 = 5.2+i3; */
  template<typename C> friend const ImageT<C> operator + (const ImageT<C> & i1, const double i2);			/**< e.g., i1 = i2+5.2; */
  template<typename C> friend const ImageT<C> operator - (const ImageT<C> & i1, const ImageT<C> & i2);	/**< e.g., i1 = i2-i3; */
  template<typename C> friend const ImageT<C> operator - (const double i1, const ImageT<C> & i2);			/**< e.g., i1 = 5.2-i3; */
  template<typename C> friend const ImageT<C> operator - (const ImageT<C> & i1, const double i2);			/**< e.g., i1 = i2-5.2; */
  template<typename C> friend const ImageT<C> operator * (const ImageT<C> & i1, const ImageT<C> & i2);	/**< e.g., i1 = i2*i3; */
  template<typename C> friend const ImageT<C> operator * (const double i1, const ImageT<C> & i2);			/**< e.g., i1 = 10*i3; */
  template<typename C> friend const ImageT<C> operator * (const ImageT<C> & i1, const double i2);			/**< e.g., i1 = i2*10; */
  template<typename C> friend const ImageT<C> operator / (const ImageT<C> & i1, const ImageT<C> & i2);  /**< e.g., i1 = i2/i3; */
  template<typename C> friend const ImageT<C> operator / (const double i1, const ImageT<C> & i2);			/**< e.g., i1 = 1/i3; */
  template<typename C> friend const ImageT<C> operator / (const ImageT<C> & i1, const double i2);			/**< e.g., i1 = i2/5; */

  //logical operators
  template<typename C> friend const ImageT<bool> operator < (const ImageT<C> & i1, const ImageT<C> & i2);	/**< e.g., iresult = i1<i2; */
  template<typename C> friend const ImageT<bool> operator < (const double i1, const ImageT<C> & i2);			/**< e.g., iresult = 2<i2; */
  template<typename C> friend const ImageT<bool> operator < (const ImageT<C> & i1, const double i2);			/**< e.g., iresult = i1<2; */
  template<typename C> friend const ImageT<bool> operator <= (const ImageT<C> & i1, const ImageT<C> & i2);	/**< e.g., iresult = i1<=i2; */
  template<typename C> friend const ImageT<bool> operator <= (const double i1, const ImageT<C> & i2);			/**< e.g., iresult = 2<=i2; */
  template<typename C> friend const ImageT<bool> operator <= (const ImageT<C> & i1, const double i2);			/**< e.g., iresult = i1<=2; */
  template<typename C> friend const ImageT<bool> operator > (const ImageT<C> & i1, const ImageT<C> & i2);	/**< e.g., iresult = i1>i2; */
  template<typename C> friend const ImageT<bool> operator > (const double i1, const ImageT<C> & i2);			/**< e.g., iresult = 2>i2; */
  template<typename C> friend const ImageT<bool> operator > (const ImageT<C> & i1, const double i2);			/**< e.g., iresult = i1>2; */
  template<typename C> friend const ImageT<bool> operator >= (const ImageT<C> & i1, const ImageT<C> & i2);	/**< e.g., iresult = i1>=i2; */
  template<typename C> friend const ImageT<bool> operator >= (const double i1, const ImageT<C> & i2);			/**< e.g., iresult = 2>=i2; */
  template<typename C> friend const ImageT<bool> operator >= (const ImageT<C> & i1, const double i2);			/**< e.g., iresult = i1>=2; */
  template<typename C> friend const ImageT<bool> operator == (const ImageT<C> & i1, const ImageT<C> & i2);	/**< e.g., iresult = i1==i2; */
  template<typename C> friend const ImageT<bool> operator == (const double i1, const ImageT<C> & i2);			/**< e.g., iresult = 2==i2; */
  template<typename C> friend const ImageT<bool> operator == (const ImageT<C> & i1, const double i2);			/**< e.g., iresult = i1==2; */

  //operations defined only on boolean images
  const ImageT<bool> operator ! () const; /**< e.g., iflag1 = !iflag2; */
  friend const ImageT<bool> operator && (const ImageT<bool> & i1, const ImageT<bool> & i2); /**< e.g., iflag1 = iflag2 && iflag3; */
  friend const ImageT<bool> operator || (const ImageT<bool> & i1, const ImageT<bool> & i2); /**< e.g., iflag1 = iflag2 || iflag3; */

  //math functions
  template<typename C> friend const ImageT<C> cos (const ImageT<C> & i1);	/**< e.g., i2 = cos(i1); */
  template<typename C> friend const ImageT<C> sin (const ImageT<C> & i1);	/**< e.g., i2 = sin(i1); */
  template<typename C> friend const ImageT<C> tan (const ImageT<C> & i1);	/**< e.g., i2 = tan(i1); */
  template<typename C> friend const ImageT<C> acos (const ImageT<C> & i1);	/**< e.g., i2 = acos(i1); */
  template<typename C> friend const ImageT<C> asin (const ImageT<C> & i1);	/**< e.g., i2 = asin(i1); */
  template<typename C> friend const ImageT<C> atan (const ImageT<C> & i1);	/**< e.g., i2 = atan(i1); */
  template<typename C> friend const ImageT<C> atan2 (const ImageT<C> & iy, const ImageT<C> & ix); /**< e.g., i2 = atan2(iy,ix); */
  template<typename C> friend const ImageT<C> cosh (const ImageT<C> & i1);	/**< e.g., i2 = cosh(i1); */
  template<typename C> friend const ImageT<C> sinh (const ImageT<C> & i1);	/**< e.g., i2 = sinh(i1); */
  template<typename C> friend const ImageT<C> tanh (const ImageT<C> & i1);	/**< e.g., i2 = tang(i1); */
  template<typename C> friend const ImageT<C> exp (const ImageT<C> & i1);		/**< e.g., i2 = exp(i1); */
  template<typename C> friend const ImageT<C> log (const ImageT<C> & i1);		/**< e.g., i2 = log(i1); */
  template<typename C> friend const ImageT<C> log10 (const ImageT<C> & i1);	/**< e.g., i2 = log10(i1); */
  template<typename C> friend const ImageT<C> abs (const ImageT<C> & i1);		/**< e.g., i2 = abs(i1); */
  template<typename C> friend const ImageT<C> ceil (const ImageT<C> & i1);	/**< e.g., i2 = ceil(i1); */
  template<typename C> friend const ImageT<C> floor (const ImageT<C> & i1);	/**< e.g., i2 = floor(i1); */
  template<typename C> friend const ImageT<C> round (const ImageT<C> & i1);	/**< e.g., i2 = round(i1); */
  template<typename C> friend const ImageT<C> mod (const ImageT<C> & i1, double d);	/**< e.g., i2 = mod(i1,5); */
  template<typename C> friend const ImageT<C> pow (const ImageT<C> & i1, double p);	/**< e.g., i2 = pow(i1,2); */
  template<typename C> friend const ImageT<C> pow (double p, const ImageT<C> & i1);	/**< e.g., i2 = pow(3,i1); */
  template<typename C> friend const ImageT<C> sqrt (const ImageT<C> & i1);			/**< e.g., i2 = sqrt(i1); */

  //filtering, convolution, and other utility functions
  template<typename C> friend const ImageT<C> convolve2D (const ImageT<C> & ikernel, const ImageT<C> & input);	//2D convolution
  template<typename C> friend const ImageT<C> filter2D (const ImageT<C> & ikernel, const ImageT<C> & input);	//2D filtering
  //need to implement separable versions for greater speed with separable filters

  template<typename C> friend const ImageT<C> medianFilter2D (const ImageT<C> & input, int filterHeight, int filterWidth); //median filter
  template<typename C> friend const ImageT<C> minFilter2D (const ImageT<C> & input, int filterHeight, int filterWidth);	 //minimum filter
  template<typename C> friend const ImageT<C> maxFilter2D (const ImageT<C> & input, int filterHeight, int filterWidth);	 //maximum filter
  template<typename C> friend const ImageT<C> meanFilter2D (const ImageT<C> & input, int filterHeight, int filterWidth);	 //mean filter

  template<typename C> friend const ImageT<C> min(const ImageT<C> & input, int dimension);	//min along a certain dimension (1,2,3 = height, width, or color respectively), e.g., i1 = min(i2,3); returns image with same height and width but 1 color channel
  template<typename C> friend const ImageT<C> max(const ImageT<C> & input, int dimension);	//max along a certain dimension (1,2,3 = height, width, or color respectively), e.g., i1 = max(i2,3); returns image with same height and width but 1 color channel
  template<typename C> friend const ImageT<C> mean(const ImageT<C> & input, int dimension);	//mean along a certain dimension (1,2,3 = height, width, or color respectively), e.g., i1 = mean(i2,3); returns image with same height and width but 1 color channel
  template<typename C> friend const ImageT<C> sum(const ImageT<C> & input, int dimension);	//sum along a certain dimension (1,2,3 = height, width, or color respectively), e.g., i1 = sum(i2,3); returns image with same height and width but 1 color channel
  template<typename C> friend C sumRegion(const ImageT<C> & input, int rowLo, int rowHi, int columnLo, int columnHi, int channelLo, int channelHi); //sum pixels in a rectangular image region
  template<typename C> friend C sumSingleChannel(const ImageT<C> & input, int channel); //sum all pixels in a single color channel
  template<typename C> friend C sumAll(const ImageT<C> & input); //sum all image pixels
  template<typename C> friend C L1Norm(const ImageT<C> & input); //sum of absolute values of all pixels
  template<typename C> friend C L2Norm(const ImageT<C> & input); //sqrt of sum of squared pixel values

  template<typename C> friend const ImageT<C> transpose(const ImageT<C> & input); //transpose image (each color channel independently)
  template<typename C> friend const ImageT<C> flipLR(const ImageT<C> & input); //flip image left to right (i.e., about vertical axis)
  template<typename C> friend const ImageT<C> flipUD(const ImageT<C> & input); //flip image upside-down (i.e., about horizontal axis)

  template<typename C> friend const ImageT<C> repmat (const ImageT<C> & input, int height, int width, int channels); //tile input image 'height' times vertically, 'width' times horizontally, and 'channels' times along color channels
  template<typename C> friend const ImageT<C> shiftImageXY (const ImageT<C> & input, int columns, int rows); //return copy of input image translated by (rows, columns)

  template<typename C> friend const ImageT<C> resizeNearestNbr(const ImageT<C> & input, double scale, bool preSmooth);	//rescale image using nearest neighbor method; use preSmooth to enable resampling
  template<typename C> friend const ImageT<C> resizeBilinear(const ImageT<C> & input, double scale, bool preSmooth);		//rescale image using bilinear interpolation method; use preSmooth to enable resampling

  //methods to create specific images and kernels	and their standalone friend versions
  void setToRandom(double lowerbound, double upperbound); //fill caller with random numbers
  friend const ImageT<double> random(double lowerbound, double upperbound, int height, int width, int nColorChannels); //create new image filled with random numbers

  void setToMeshgridX (T x1, T x2, T y1, T y2, T dx = 1, T dy = 1); //set caller to an image of height y2-y1+1 and width x2-x1+1 with each pixel set to its x-coordinate (from x1 to x2)
  friend const ImageT<double> meshgridX (double x1, double x2, double y1, double y2, double dx, double dy); //create new image of height y2-y1+1 and width x2-x1+1 with each pixel set to its x-coordinate (from x1 to x2)

  void setToMeshgridY (T x1, T x2, T y1, T y2, T dx = 1, T dy = 1); //set caller to an image of height y2-y1+1 and width x2-x1+1 with each pixel set to its y-coordinate (from y1 to y2)
  friend const ImageT<double> meshgridY (double x1, double x2, double y1, double y2, double dx, double dy); //create new image of height y2-y1+1 and width x2-x1+1 with each pixel set to its y-coordinate (from y1 to y2)

  void setToGaussian(int size, double sigma);	 //set caller to a gaussian
  friend const ImageT<double> gaussian(int size, double sigma);	 //create a new gaussian

  void setToGaborX(int size, double sigma, double period, double phaseshift);	//set caller to a gabor filter oriented horizontally
  friend const ImageT<double> gaborX(int size, double sigma, double period, double phaseshift);	//create a gabor filter oriented horizontally

  void setToGaborY(int size, double sigma, double period, double phaseshift);	//set caller to a gabor filter oriented vertically
  friend const ImageT<double> gaborY(int size, double sigma, double period, double phaseshift);	//create a gabor filter oriented vertically

  void setToGaborOriented(int size, double sigma, double period, double angle, double phaseshift); //set caller to a gabor filter with a user-specified orientation
  friend const ImageT<double> gaborOriented(int size, double sigma, double period, double angle, double phaseshift);	//create a gabor filter with a user-specified orientation

  ImageT<double> getGaborPhaseStack();

  void setToGray();	//convert caller to gray image (single channel)
  template<typename C> friend const ImageT<C> rgb2gray(const ImageT<C> & input);	 //convert color image (multiple channels) to gray image (single channel)

  template<typename C> friend bool haveEqualDimensions (const ImageT<C> & i1, const ImageT<C> & i2); // Returns true if the two input images have the same height, width and channels
  template<typename C> friend bool haveEqualHeightWidth (const ImageT<C> & i1, const ImageT<C> & i2); // Returns true if the two input images have the same height and width, ignores number of channels

  //desired functionality:
  //imtransform (for general transformation matrix)
  //generalized nonlinear block filter
  //separable filtering


protected:
  int  mHeight;			/**<height of the image*/
  int  mWidth;			/**<width of the image*/
  int  mChannels;			/**<number of color channels or dimensions (e.g., 1 for grayscale, 3 for RGB)*/

  //for convenience
  int  mHeightTimesWidth;	/**< mWidth*mHeight (for convenience) */
  int  mSize;				/**< mWidth*mHeight*mChannels (for convenience) */

  T   *mData;				/**< Image data */
};

/**  Rounds to nearest integer
* @param value	input value
* @return the rounded value
*/
inline int ov_round(double value)
{
  return int(value + 0.5);
}

/** 
* Constructor with no parameters to create empty image.
* @see ImageT(int height, int width, int nColorChannels)
* @see ImageT(const ImageT<T>& srcImage, bool copyData)
* @see ImageT(const ImageT<C>& srcImage, bool copyData)
*/
template<typename T>
ImageT<T>::ImageT()
: mHeight(0), mWidth(0), mChannels(0), mHeightTimesWidth(0), mSize(0), mData(0)
{
}

/** 
* Constructor specifying height, width and number of color channels.
* 
* @param height	desired height of the image
* @param width desired width of image
* @param nColorChannels desired number of color channels
* @see ImageT()
* @see ImageT(const ImageT<T>& srcImage, bool copyData)
* @see ImageT(const ImageT<C>& srcImage, bool copyData)
*/
template<typename T>
ImageT<T>::ImageT(int height, int width, int nColorChannels)
: mHeight(0), mWidth(0), mChannels(0), mHeightTimesWidth(0), mSize(0), mData(0)
{
  if(height>0) mHeight = height;
  if(width>0) mWidth = width;
  if(nColorChannels>0) mChannels = nColorChannels;
  mSize = mWidth*mHeight*mChannels;	

  if(mSize>0) 
  {
    mData = new T[mSize];
    if(mData == 0)
    {
      mHeight = 0;
      mWidth = 0;
      mChannels = 0;
      mSize = 0;
    }
    else
    {
      for(int i=0; i<mSize; i++) mData[i] = (T) 0;
    }
  }

  mHeightTimesWidth = mWidth*mHeight;
}

/** 
* Copy constructor creates image with same size as the input image, with the option of copying data.
* 
* @param srcImage source image whose dimensions are to be copied
* @param copyData copies source data if set to true (default), otherwise returns zeroed image.
* @see ImageT()
* @see ImageT(int height, int width, int nColorChannels)
* @see ImageT(const ImageT<C>& srcImage, bool copyData)
*/
template<typename T>
ImageT<T>::ImageT(const ImageT<T>& srcImage, bool copyData)
: mHeight(srcImage.mHeight), mWidth(srcImage.mWidth), mChannels(srcImage.mChannels), mHeightTimesWidth(srcImage.mHeightTimesWidth), mSize(srcImage.mSize)
{
  if((srcImage.mSize>0) && (srcImage.mData != 0))
  {
    mData = new T[mSize];
    if(mData == 0)
    {
      mHeight = 0;
      mWidth = 0;
      mChannels = 0;
      mSize = 0;
      mHeightTimesWidth = 0;
    }
    else
    {
      if(copyData)
      {
        for(int i=0;i<mSize; i++) mData[i] = srcImage.mData[i];
      }
      else
      {
        for(int i=0;i<mSize; i++) mData[i] = 0;
      }
    }
  }
  else
  {
    mHeight = 0;
    mWidth = 0;
    mChannels = 0;
    mSize = 0;
    mHeightTimesWidth = 0;
    mData = 0;
  }
}

/** 
* Copy constructor to copy between two different image template types T and C.
* <p>e.g., 
* <pre>
*   ImageT<int> i1(2,3,1);
*   ImageT<float> i2(i1);
* </pre>
*
* @param srcImage source image whose dimensions are to be copied (of a different template type)
* @param copyData copies source data if set to true (default), otherwise returns zeroed image.
* @see ImageT()
* @see ImageT(int height, int width, int nColorChannels)
* @see ImageT(const ImageT<T>& srcImage, bool copyData)
*/
template<typename T>
template<typename C>
ImageT<T>::ImageT(const ImageT<C>& srcImage, bool copyData)
{
  int height,width,ncolors,srcSize;

  srcImage.getDimensions(height,width,ncolors);
  srcSize = height*width*ncolors;

  mHeight = height;
  mWidth = width;
  mChannels= ncolors;
  mSize = srcSize;
  mHeightTimesWidth = mHeight*mWidth;

  if(srcSize!=0)
  {
    mData = new T[mSize];
    if(mData == 0)
    {
      mHeight = 0;
      mWidth = 0;
      mChannels = 0;
      mSize = 0;
      mHeightTimesWidth = 0;
    }
    else
    {
      if(copyData)
      {
        for(int k=0;k<mChannels;k++)
          for(int j=0;j<mWidth;j++)
            for(int i=0;i<mHeight;i++)
              (*this)(i,j,k) = (T) srcImage(i,j,k); 
      }
      else
      {
        for(int i=0;i<mSize; i++) mData[i] = 0;
      }
    }
  }
  else
  {
    mHeight = 0;
    mWidth = 0;
    mChannels = 0;
    mSize = 0;
    mHeightTimesWidth = 0;
    mData = 0;
  }
}

/** 
* Destructor of ImageT.
*/
template<typename T>
ImageT<T>::~ImageT()
{
  if(mData!=0) delete [] mData;
}

/**
* Imports image from an ImageAdapter (external image source).
* @param iadapter a ImageAdapter object
* @return true if successful, false if failed
*
* <BR> e.g., import and export using OpenCVAdapter, a class derived from ImageAdapter.
* <pre>
*    IplImage*img = cvLoadImage("test.jpg");
*    OpenCVImageAdapter*opencvAdaptor = new OpenCVImageAdapter(img);
*    ImageT<float> i1;
*    i1.copyFromAdapter(*opencvAdaptor); //Now the opencv image is copied to i1
*    i1 = i1/2;	//divide all pixel values by 2
*    i1.copyToAdapter(*opencvAdaptor); //copy back to opencv image
* </pre>
* @see copyToAdapter(ImageAdapter & iadapter)
*/
template<typename T>
bool ImageT<T>::copyFromAdapter(const ImageAdapter & iadapter)
{
  int i,j,k;
  int height, width, ncolors;
  iadapter.getSize(height, width, ncolors);
  resetDimensions(height,width,ncolors);

  if((height!=mHeight)||(width!=mWidth)||(ncolors!=mChannels)) return false;

  for(k=0; k<mChannels;k++)
    for(j=0; j<mWidth;j++)
      for(i=0; i<mHeight;i++)
      {
        (*this)(i,j,k) = (T) iadapter.getPixel(i,j,k); 
      }

      return true;
}

/**
* Exports image to an ImageAdapter (external image source).
* Note: This function will export only if the ImageAdapter image has the same dimensions as the source image.
* It does not create or resize the ImageAdapter.
* @param iadapter a ImageAdapter object
* @return true if successful, false if failed
*
* <BR> e.g., import and export using OpenCVAdapter, a class derived from ImageAdapter.
* <pre>
*    IplImage*img = cvLoadImage("test.jpg");
*    OpenCVImageAdapter*opencvAdaptor = new OpenCVImageAdapter(img);
*    ImageT<float> i1;
*    i1.copyFromAdapter(*opencvAdaptor); //Now the opencv image is copied to i1
*    i1 = i1/2;	//divide all pixel values by 2
*    i1.copyToAdapter(*opencvAdaptor); //copy back to opencv image
* </pre>
* @see copyFromAdapter(ImageAdapter & iadapter)
*/
template<typename T>
bool ImageT<T>::copyToAdapter(ImageAdapter & iadapter)
{
  int i,j,k;
  int height, width, ncolors;
  iadapter.getSize(height, width, ncolors);

  if((height!=mHeight)||(width!=mWidth)||(ncolors!=mChannels)) return false;

  for(k=0; k<mChannels;k++) 
    for(j=0; j<mWidth;j++)
      for(i=0; i<mHeight;i++)
      {
        iadapter.setPixel((double)(*this)(i,j,k),i,j,k);
      }

      return true;
}

/**
* Returns the dimensions of the image.
* @param height returns image height
* @param width returns image width
* @param nColorChannels returns number of channels
*/
template<typename T>
void ImageT<T>::getDimensions(int & height, int & width, int & nColorChannels) const
{
  height = mHeight;
  width = mWidth;
  nColorChannels = mChannels;
}

/**
* Returns the height of the image.
* @return height of the image
*/
template<typename T>
int ImageT<T>::getHeight() const
{
  return mHeight;
}

/**
* Returns the width of the image.
* @return width of the image
*/
template<typename T>
int ImageT<T>::getWidth() const
{
  return mWidth;
}

/**
* Returns the number of channels of the image.
* @return number of channels
*/
template<typename T>
int ImageT<T>::getChannels() const
{
  return mChannels;
}


/**
* Resets the image to the specified dimensions and zeroes all pixel values.
* e.g.,
* <pre>
*   i1.resetDimensions(5,10,3);
* </pre>
* @param height image height
* @param width image width
* @param nColorChannels number of channels
*/
template<typename T>
void ImageT<T>::resetDimensions(int height, int width, int nColorChannels)
{
  if(mData!=0) {delete [] mData; mData = 0;}

  if(height>0) mHeight = height; else height = 0;
  if(width>0) mWidth = width; else width = 0;
  if(nColorChannels>0) mChannels = nColorChannels; else nColorChannels = 0;
  mSize = mWidth*mHeight*mChannels;	

  if(mSize>0) 
  {
    mData = new T[mSize];
    if(mData == 0)
    {
      mHeight = 0;
      mWidth = 0;
      mChannels = 0;
      mSize = 0;
    }
    else
    {
      for(int i=0; i<mSize; i++) mData[i] = (T) 0;
    }
  }

  mHeightTimesWidth = mWidth*mHeight;
}

/**
* Reshapes the image without changing total size or pixel values.
* Alters the height, width and channels only if their product is equal to the product of the existing values.
* <br> i.e., only if height*width*channels = (old height)*(old width)*(old channels)
* <br> e.g,
* <pre>
*   ImageT<float> i1(2,8,1);
*   i1.reshape(4,4,1);
* </pre>
* @param height image height
* @param width image width
* @param nColorChannels number of channels
*/
template<typename T>
void ImageT<T>::reshape(int height, int width, int nColorChannels)
{
  if((height*width*nColorChannels)==mSize)
  {
    mHeight = height;
    mWidth = width;
    mChannels = nColorChannels;
    mHeightTimesWidth = mWidth*mHeight;
  }
}

/**
* Rescale image intensities to lie between 0 and 1.
* <br> e.g,
* <pre>
*   i1.normalizeIntensityRange();
* </pre>
*/
template<typename T>
void ImageT<T>::normalizeIntensityRange()
{
  int i;
  T minValue, maxValue, range; 
  if(mSize==0) return;

  minValue = maxValue = mData[0];
  for(i=1; i<mSize; i++)
  {
    if(minValue>mData[i])minValue = mData[i];
    if(maxValue<mData[i])maxValue = mData[i];
  }
  if(minValue==maxValue) return; //to avoid divide by zero later
  range = maxValue-minValue;

  for(i=0; i<mSize; i++) mData[i] = (mData[i]-minValue)/range;
}



/** Access image data using a syntax like im(i,j,k) .
* <p> e.g., 
* <br> temp = im(1,2,1); //copy value of pixel at location row=1,col=2,channel=1 into variable temp
* <br> OR
* <br> im(1,2,0) = 5;
*
* @param row 
* @param column 
* @param channel
*/
template<typename T>
inline T& ImageT<T>::operator() (int row, int column, int channel)
{
  return *(mData + channel*mHeightTimesWidth + column*mHeight + row); 
}

/*
Const version of the above operator.
*/
template<typename T>
inline T& ImageT<T>::operator() (int row, int column, int channel) const
{
  return *(mData + channel*mHeightTimesWidth + column*mHeight + row); 
}

/**
* Assignment operator to copy one image to another of the same data type.
* e.g.,
* <pre>
*    ImageT<float> i1(4,4,1);
*    ImageT<float> i2;
*    i2 = i1;
* </pre>
*/
template<typename T>
ImageT<T>& ImageT<T>::operator = (const ImageT<T> & rhsImage)
{
  if(this == &rhsImage) return (*this); // for a case where lhsimage == rhsimage

  if((rhsImage.mSize>0) && (rhsImage.mData != 0))
  {
    if(mSize != rhsImage.mSize) //reallocate memory only if size is different, else just use what is already there
    {
      if(mData!=0) {delete [] mData; mData = 0;}
      mSize = rhsImage.mSize;
      mData = new T[mSize];
    }

    if(mData == 0)
    {
      mHeight = 0;
      mWidth = 0;
      mChannels = 0;
      mSize = 0;
      mHeightTimesWidth = 0;
    }
    else
    {
      mHeight = rhsImage.mHeight;
      mWidth = rhsImage.mWidth;
      mChannels = rhsImage.mChannels; 
      mHeightTimesWidth = rhsImage.mHeightTimesWidth;

      for(int i=0;i<mSize; i++) mData[i] = rhsImage.mData[i];
    }
  }
  return (*this);
}

/**
* Assignment operator to copy one image to another of a different data type.
* e.g.,
* <pre>
*    ImageT<float> i1(4,4,1);
*    ImageT<int> i2;
*    i2 = i1;
* </pre>
*/
template<typename T>
template<typename C>
ImageT<T>& ImageT<T>::operator = (const ImageT<C> & rhsImage)
{
  //self-assignment check not necessary since T and C are different, hence not used below

  int height,width,ncolors,rhssize;

  rhsImage.getDimensions(height,width,ncolors);
  rhssize = height*width*ncolors;

  if(rhssize!=0)
  {
    if(mSize != rhssize) //reallocate memory only if size is different, else just use what is already there
    {
      if(mData!=0) {delete [] mData; mData = 0;}
      mSize = rhssize;
      mData = new T[mSize];
    }

    if(mData == 0)
    {
      mHeight = 0;
      mWidth = 0;
      mChannels = 0;
      mSize = 0;
      mHeightTimesWidth = 0;
    }
    else
    {
      mHeight = height;
      mWidth = width;
      mChannels = ncolors; 
      mHeightTimesWidth = mHeight*mWidth;

      for(int k=0;k<mChannels;k++)
        for(int j=0;j<mWidth;j++)
          for(int i=0;i<mHeight;i++)
            (*this)(i,j,k) = (T) rhsImage(i,j,k); 
    }
  }
  return (*this);
}

/**
* Assignment operator to set all values of an image to a given scalar.
* e.g.,
* <pre>
*    ImageT<float> i1(4,4,1);
*    i1 = 4.2;
* </pre>
*/
template<typename T>
ImageT<T>& ImageT<T>::operator = (const T & rhs)
{
  for(int i=0;i<mSize; i++) mData[i] = rhs;
  return (*this);
}

/**
* Import source image values using a boolean image mask. Values are copied wherever mask is true.
* @param mask a boolean image mask having the same size as the srcImage
* @param srcImage the source image
* @return true if successful
*/
template<typename T>
bool ImageT<T>::copyMasked(const ImageT<bool> & mask, const ImageT<T> & srcImage)
{
  int maskHeight, maskWidth, maskChannels, srcHeight, srcWidth, srcChannels;

  mask.getDimensions(maskHeight,maskWidth,maskChannels);
  srcImage.getDimensions(srcHeight,srcWidth,srcChannels);

  if((mHeight!=maskHeight)||(mWidth!=maskWidth)||(mChannels!=maskChannels)) return false;		
  if((mHeight!=srcHeight)||(mWidth!=srcWidth)||(mChannels!=srcChannels)) return false;		

  for(int k=0; k<mChannels;k++)
    for(int j=0; j<mWidth;j++)
      for(int i=0; i<mHeight;i++)
      {
        if(mask(i,j,k)) (*this)(i,j,k) = srcImage(i,j,k);
      }

      return true;
}

/**
* Sets pixels equal to the input value wherever the mask is true.
* @param mask a boolean image mask having the same size as the srcImage
* @param value the scalar value to be assigned
* @return true if successful
*/
template<typename T>
bool ImageT<T>::copyMasked(const ImageT<bool> & mask, const T & value)
{
  int maskHeight, maskWidth, maskChannels;
  mask.getDimensions(maskHeight,maskWidth,maskChannels);
  if((mHeight!=maskHeight)||(mWidth!=maskWidth)||(mChannels!=maskChannels)) return false;		

  for(int k=0; k<mChannels;k++)
    for(int j=0; j<mWidth;j++)
      for(int i=0; i<mHeight;i++)
      {
        if(mask(i,j,k)) (*this)(i,j,k) = value;
      }
      return true;
}

/**
* Copies a certain input channel to a certain output channel.
* Note: the input should have the same height and width as the caller.
* @param input the input image
* @param inputChannel the channel of the input to be copied
* @param outputChannel the destination channel of the output
* @return true if successful
*/
//
template<typename T>
bool ImageT<T>::copyChannel(ImageT<T> & input, int inputChannel, int outputChannel)
{
  if((mHeight!=input.mHeight)||(mWidth!=input.mWidth)) return false;		
  if((input.mChannels>=inputChannel)||(inputChannel<0)) return false;		
  if((mChannels>=outputChannel)||(outputChannel<0)) return false;		

  for(int j=0; j<mWidth; j++)
    for(int i=0; i<mHeight; i++)
      (*this)(i,j,outputChannel) = (T) (input(i,j,inputChannel));

  return true;
}


/**
* Copies and returns a rectangular sub-block of the image.
* e.g.,
* <pre>
*    i2 = i1(10,20,5,30,0,1); 
* </pre>
* @param rowLo starting row (if omitted or set to a negative value, the default value of 0 is used)
* @param rowHi ending row (if omitted or set to a negative value, the default value used is (height-1) )
* @param columnLo starting column (if omitted or set to a negative value, the default value of 0 is used)
* @param columnHi ending column (if omitted or set to a negative value, the default value used is (width-1))
* @param channelLo starting channel (if omitted or set to a negative value, the default value of 0 is used)
* @param channelHi ending channel (if omitted or set to a negative value, the default value used is (number of channels-1))
* @return the copied subimage
*/
template<typename T>
const ImageT<T> ImageT<T>::getSubImage(int rowLo, int rowHi, int columnLo, int columnHi, int channelLo, int channelHi)
{
  int i,j,k,width,height,nchannels;
  ImageT<T> result;

  if(rowLo<0) rowLo = 0; if(rowLo>=mHeight) rowLo = mHeight-1;
  if(rowHi<0) rowHi = mHeight-1; if(rowHi>=mHeight) rowHi = mHeight-1;	
  if(columnLo<0) columnLo = 0; if(columnLo>=mWidth) columnLo = mWidth-1;
  if(columnHi<0) columnHi = mWidth-1; if(columnHi>=mWidth) columnHi = mWidth-1;	
  if(channelLo<0) channelLo = 0; if(channelLo>=mChannels) channelLo = mChannels-1;
  if(channelHi<0) channelHi = mChannels-1; if(channelHi>=mChannels) channelHi = mChannels-1;	

  height = rowHi-rowLo+1;
  width  = columnHi-columnLo+1;
  nchannels = channelHi-channelLo+1;

  if(height<=0) return result;
  if(width<=0) return result;
  if(nchannels<=0) return result;

  result.resetDimensions(height,width,nchannels);

  for(k=channelLo;k<=channelHi;k++)
    for(j=columnLo;j<=columnHi;j++)
      for(i=rowLo;i<=rowHi;i++)
        result(i-rowLo,j-columnLo,k-channelLo) = (*this)(i,j,k);

  return result;
}


template<typename T>
void ImageT<T>::print(void)
{
  using namespace std;
  int i,j,k;

  cout << "Height: " << mHeight << ",Width:" << mWidth << ",Channels:" << mChannels << endl;

  for(k=0;k<mChannels;k++)
  {
    cout << "Channel " << k << endl;
    for(i=0;i<mHeight;i++)
    {
      for(j=0;j<mWidth;j++)
      {
        cout << (*this)(i,j,k) << "\t";
      }
      cout << endl;
    }
  }
}

template<typename T>
ImageT<T> & ImageT<T>::operator += (const ImageT<T> & rhs)
{
  if((mHeight==rhs.mHeight)&&(mWidth==rhs.mWidth)&&(mChannels==rhs.mChannels))
  {
    for(int i=0; i<mSize; i++)
      mData[i] += rhs.mData[i];
  }
  return (*this);
}

template<typename T>
ImageT<T> & ImageT<T>::operator += (const T & rhs)
{
  for(int i=0; i<mSize; i++)
    mData[i] += rhs;

  return (*this);
}

template<typename T>
ImageT<T> & ImageT<T>::operator -= (const ImageT<T> & rhs)
{
  if((mHeight==rhs.mHeight)&&(mWidth==rhs.mWidth)&&(mChannels==rhs.mChannels))
  {
    for(int i=0; i<mSize; i++)
      mData[i] -= rhs.mData[i];
  }
  return (*this);
}

template<typename T>
ImageT<T> & ImageT<T>::operator -= (const T & rhs)
{
  for(int i=0; i<mSize; i++)
    mData[i] -= rhs;

  return (*this);
}

template<typename T>
ImageT<T> & ImageT<T>::operator *= (const ImageT<T> & rhs)
{
  if((mHeight==rhs.mHeight)&&(mWidth==rhs.mWidth)&&(mChannels==rhs.mChannels))
  {
    for(int i=0; i<mSize; i++)
      mData[i] *= rhs.mData[i];
  }
  return (*this);
}

template<typename T>
ImageT<T> & ImageT<T>::operator *= (const T & rhs)
{
  for(int i=0; i<mSize; i++)
    mData[i] *= rhs;

  return (*this);
}

template<typename T>
ImageT<T> & ImageT<T>::operator /= (const ImageT<T> & rhs)
{
  if((mHeight==rhs.mHeight)&&(mWidth==rhs.mWidth)&&(mChannels==rhs.mChannels))
  {
    for(int i=0; i<mSize; i++)
      mData[i] /= rhs.mData[i];
  }
  return (*this);
}

template<typename T>
ImageT<T> & ImageT<T>::operator /= (const T & rhs)
{
  for(int i=0; i<mSize; i++)
    mData[i] /= rhs;

  return (*this);
}

template<typename T>
ImageT<T> & ImageT<T>::operator ++ ()
{
  for(int i=0; i<mSize; i++)
    mData[i]++;

  return (*this);
}

template<typename T>
ImageT<T> & ImageT<T>::operator -- ()
{
  for(int i=0; i<mSize; i++)
    mData[i]--;

  return (*this);
}

template<typename T>
const ImageT<T> ImageT<T>::operator ++ (int)
{
  ImageT<T> oldImage(*this); //make a copy
  for(int i=0; i<mSize; i++) 
    ++mData[i];				//increment original

  return (oldImage); //return the copy
}

template<typename T>
const ImageT<T> ImageT<T>::operator -- (int)
{
  ImageT<T> oldImage(*this); //make a copy
  for(int i=0; i<mSize; i++)
    --mData[i];			//decrement original

  return (oldImage); //return the copy
}

template<typename T>
const ImageT<T> ImageT<T>::operator - ()
{
  ImageT<T> result(*this); 

  for(int i=0; i<mSize; i++)
    result.mData[i] = -result.mData[i];

  return (result); 
}

template<typename T>
const ImageT<T> ImageT<T>::operator + ()
{
  ImageT<T> result(*this); 
  return (result); 
}


template<typename T>
const ImageT<T> operator + (const ImageT<T> & i1, const ImageT<T> & i2)
{
  ImageT<T> result;
  result = i1;
  result += i2;
  return result;
}

template<typename T>
const ImageT<T> operator + (const double i1, const ImageT<T> & i2)
{
  ImageT<T> result;
  result = i2;
  result += (T)i1;
  return result;
}

template<typename T>
const ImageT<T> operator + (const ImageT<T> & i1, const double i2)
{
  ImageT<T> result;
  result = i1;
  result += (T)i2;
  return result;
}

template<typename T>
const ImageT<T> operator - (const ImageT<T> & i1, const ImageT<T> & i2)
{
  ImageT<T> result;
  result = i1;
  result -= i2;
  return result;
}

template<typename T>
const ImageT<T> operator - (const double i1, const ImageT<T> & i2)
{
  ImageT<T> result;
  result = i2;
  result -= (T)i1;
  return result;
}

template<typename T>
const ImageT<T> operator - (const ImageT<T> & i1, const double i2)
{
  ImageT<T> result;
  result = i1;
  result -= (T)i2;
  return result;
}

template<typename T>
const ImageT<T> operator * (const ImageT<T> & i1, const ImageT<T> & i2)
{
  ImageT<T> result;
  result = i1;
  result *= i2;
  return result;
}

template<typename T>
const ImageT<T> operator * (const double i1, const ImageT<T> & i2)
{
  ImageT<T> result;
  result = i2;
  result *= (T)i1;
  return result;
}

template<typename T>
const ImageT<T> operator * (const ImageT<T> & i1, const double i2)
{
  ImageT<T> result;
  result = i1;
  result *= (T)i2;
  return result;
}

template<typename T>
const ImageT<T> operator / (const ImageT<T> & i1, const ImageT<T> & i2)
{
  ImageT<T> result;
  result = i1;
  result /= i2;
  return result;
}

template<typename T>
const ImageT<T> operator / (const double i1, const ImageT<T> & i2)
{
  ImageT<T> result;
  result = i2;
  result /= (T)i1;
  return result;
}

template<typename T>
const ImageT<T> operator / (const ImageT<T> & i1, const double i2)
{
  ImageT<T> result;
  result = i1;
  result /= (T)i2;
  return result;
}

template<typename T>
const ImageT<bool> operator < (const ImageT<T> & i1, const ImageT<T> & i2)
{
  ImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

  if((i1.mHeight==i2.mHeight) && (i1.mWidth==i2.mWidth) && (i1.mChannels==i2.mChannels))
  {
    for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] < i2.mData[i]);
  }

  return result;
}

template<typename T>
const ImageT<bool> operator < (const double i1, const ImageT<T> & i2)
{
  ImageT<bool> result(i2.mHeight,i2.mWidth,i2.mChannels);

  for(int i=0; i<result.mSize;i++) result.mData[i] = (i1 < i2.mData[i]);

  return result;
}

template<typename T>
const ImageT<bool> operator < (const ImageT<T> & i1, const double i2)
{
  ImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

  for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] < i2);

  return result;
}


template<typename T>
const ImageT<bool> operator <= (const ImageT<T> & i1, const ImageT<T> & i2)
{
  ImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

  if((i1.mHeight==i2.mHeight) && (i1.mWidth==i2.mWidth) && (i1.mChannels==i2.mChannels))
  {
    for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] <= i2.mData[i]);
  }

  return result;
}

template<typename T>
const ImageT<bool> operator <= (const double i1, const ImageT<T> & i2)
{
  ImageT<bool> result(i2.mHeight,i2.mWidth,i2.mChannels);

  for(int i=0; i<result.mSize;i++) result.mData[i] = (i1 <= i2.mData[i]);

  return result;
}

template<typename T>
const ImageT<bool> operator <= (const ImageT<T> & i1, const double i2)
{
  ImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

  for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] <= i2);

  return result;
}


template<typename T>
const ImageT<bool> operator > (const ImageT<T> & i1, const ImageT<T> & i2)
{
  ImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

  if((i1.mHeight==i2.mHeight) && (i1.mWidth==i2.mWidth) && (i1.mChannels==i2.mChannels))
  {
    for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] > i2.mData[i]);
  }

  return result;
}

template<typename T>
const ImageT<bool> operator > (const double i1, const ImageT<T> & i2)
{
  ImageT<bool> result(i2.mHeight,i2.mWidth,i2.mChannels);

  for(int i=0; i<result.mSize;i++) result.mData[i] = (i1 > i2.mData[i]);

  return result;
}

template<typename T>
const ImageT<bool> operator > (const ImageT<T> & i1, const double i2)
{
  ImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

  for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] > i2);

  return result;
}


template<typename T>
const ImageT<bool> operator >= (const ImageT<T> & i1, const ImageT<T> & i2)
{
  ImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

  if((i1.mHeight==i2.mHeight) && (i1.mWidth==i2.mWidth) && (i1.mChannels==i2.mChannels))
  {
    for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] >= i2.mData[i]);
  }

  return result;
}

template<typename T>
const ImageT<bool> operator >= (const double i1, const ImageT<T> & i2)
{
  ImageT<bool> result(i2.mHeight,i2.mWidth,i2.mChannels);

  for(int i=0; i<result.mSize;i++) result.mData[i] = (i1 >= i2.mData[i]);

  return result;
}

template<typename T>
const ImageT<bool> operator >= (const ImageT<T> & i1, const double i2)
{
  ImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

  for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] >= i2);

  return result;
}

template<typename T>
const ImageT<bool> operator == (const ImageT<T> & i1, const ImageT<T> & i2)
{
  ImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

  if((i1.mHeight==i2.mHeight) && (i1.mWidth==i2.mWidth) && (i1.mChannels==i2.mChannels))
  {
    for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] == i2.mData[i]);
  }

  return result;
}

template<typename T>
const ImageT<bool> operator == (const double i1, const ImageT<T> & i2)
{
  ImageT<bool> result(i2.mHeight,i2.mWidth,i2.mChannels);

  for(int i=0; i<result.mSize;i++) result.mData[i] = (i1 == i2.mData[i]);

  return result;
}

template<typename T>
const ImageT<bool> operator == (const ImageT<T> & i1, const double i2)
{
  ImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

  for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] == i2);

  return result;
}

template<typename T> 
const ImageT<T> cos (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) cos(i1.mData[i]);
  return result;
}

template<typename T> 
const ImageT<T> sin (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) sin(i1.mData[i]);
  return result;
}


template<typename T> 
const ImageT<T> tan (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) tan(i1.mData[i]);
  return result;
}


template<typename T> 
const ImageT<T> acos (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) acos(i1.mData[i]);
  return result;
}


template<typename T> 
const ImageT<T> asin (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) asin(i1.mData[i]);
  return result;
}


template<typename T> 
const ImageT<T> atan (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) atan(i1.mData[i]);
  return result;
}


template<typename T> 
const ImageT<T> atan2 (const ImageT<T> & iy, const ImageT<T> & ix)
{
  ImageT<T> result(iy,false); //create same-sized copy without copying contents
  if(iy.mSize != ix.mSize) return result; //unequal sized arrays
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) atan2(iy.mData[i],ix.mData[i]);
  return result;
}


template<typename T> 
const ImageT<T> cosh (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) cosh(i1.mData[i]);
  return result;
}


template<typename T> 
const ImageT<T> sinh (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) sinh(i1.mData[i]);
  return result;
}


template<typename T> 
const ImageT<T> tanh (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) tanh(i1.mData[i]);
  return result;
}


template<typename T> 
const ImageT<T> exp (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) exp(i1.mData[i]);
  return result;
}


template<typename T> 
const ImageT<T> log (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) log(i1.mData[i]);
  return result;
}


template<typename T> 
const ImageT<T> log10 (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) log10(i1.mData[i]);
  return result;
}


template<typename T> 
const ImageT<T> abs (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) fabs(i1.mData[i]);
  return result;
}


template<typename T> 
const ImageT<T> ceil (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) ceil(i1.mData[i]);
  return result;
}


template<typename T> 
const ImageT<T> floor (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) floor(i1.mData[i]);
  return result;
}

template<typename T> 
const ImageT<T> round (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  T tempValue;
  for(int i=0; i<result.mSize; i++) 
  {
    tempValue = floor(i1.mData[i]);
    result.mData[i] = (T) ((i1.mData[i]-tempValue)<=0.5)?tempValue:ceil(i1.mData[i]);
  }
  return result;
}

template<typename T> 
const ImageT<T> mod (const ImageT<T> & i1, double d)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) fmod((double)i1.mData[i],(double)d);
  return result;
}


template<typename T> 
const ImageT<T> pow (const ImageT<T> & i1, double p)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) pow((double)i1.mData[i],p);
  return result;
}

template<typename T> 
const ImageT<T> pow (double p, const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) pow(p,(double)i1.mData[i]);
  return result;
}

template<typename T> 
const ImageT<T> sqrt (const ImageT<T> & i1)
{
  ImageT<T> result(i1,false); //create same-sized copy without copying contents
  for(int i=0; i<result.mSize; i++) result.mData[i] = (T) sqrt(i1.mData[i]);
  return result;
}


/** 
* @relates ImageT
* Performs 2D convolution on the input image with a given kernel.
* <br> e.g., 
* <pre>
*   ImageT<float> ikernel, input, iresult;
*   input = random(0,100,50,50,1);
*   ikernel = gaussian(7, 3);
*   iresult = convolve2D(ikernel, input);
* </pre>
* @param kernel a 2D image used as a kernel for convolution
* @param input the input image
* @return convolved image
* @see filter2D(const ImageT<T> & kernel, const ImageT<T> & input)
*/
template<typename T> 
const ImageT<T> convolve2D (const ImageT<T> & kernel, const ImageT<T> & input)
{
  int iResult,jResult,k,iKernel,jKernel,iInput,jInput,iKernelMidpoint,jKernelMidpoint;
  T tempValue;

  ImageT<T> result(input,false); //create same-sized copy without copying contents

  if((kernel.mSize==0)||(input.mSize==0)) return result;

  iKernelMidpoint = kernel.mHeight/2;
  jKernelMidpoint = kernel.mWidth/2;

  for(k=0;k<result.mChannels;k++)
    for(jResult=0;jResult<result.mWidth;jResult++)
      for(iResult=0;iResult<result.mHeight;iResult++)
      {
        tempValue = 0;

        for(jKernel=0; jKernel<kernel.mWidth; jKernel++)
        {
          jInput = jResult - (jKernel-jKernelMidpoint);
          if((jInput<0)||(jInput>=input.mWidth)) continue;

          for(iKernel=0; iKernel<kernel.mHeight; iKernel++)
          {
            iInput = iResult - (iKernel-iKernelMidpoint);
            if((iInput<0)||(iInput>=input.mHeight)) continue;
            tempValue += (kernel(iKernel,jKernel)*input(iInput,jInput,k));
          }
        }

        result(iResult,jResult,k) = tempValue;
      }

      return result;
}


/** 
* @relates ImageT
* Performs 2D filtering on the input image with a given kernel.
* <br> e.g., 
* <pre>
*   ImageT<float> ikernel, input, iresult;
*   input = random(0,100,50,50,1);
*   ikernel = gaussian(7, 3);
*   iresult = filter2D(ikernel, input);
* </pre>
* @param kernel a 2D image used as a kernel for filtering
* @param input the input image
* @return filtered image
* @see convolve2D(const ImageT<T> & kernel, const ImageT<T> & input)
*/
template<typename T> 
const ImageT<T> filter2D (const ImageT<T> & kernel, const ImageT<T> & input)
{
  int iResult,jResult,k,iKernel,jKernel,iInput,jInput,iKernelMidpoint,jKernelMidpoint;
  T tempValue;

  ImageT<T> result(input,false); //create same-sized copy without copying contents

  if((kernel.mSize==0)||(input.mSize==0)) return result;

  iKernelMidpoint = kernel.mHeight/2;
  jKernelMidpoint = kernel.mWidth/2;

  for(k=0;k<result.mChannels;k++)
    for(jResult=0;jResult<result.mWidth;jResult++)
      for(iResult=0;iResult<result.mHeight;iResult++)
      {
        tempValue = 0;

        for(jKernel=0; jKernel<kernel.mWidth; jKernel++)
        {
          jInput = jResult + (jKernel-jKernelMidpoint); //only difference from convolve2D is the plus sign
          if((jInput<0)||(jInput>=input.mWidth)) continue;

          for(iKernel=0; iKernel<kernel.mHeight; iKernel++)
          {
            iInput = iResult + (iKernel-iKernelMidpoint); //only difference from convolve2D is the plus sign
            if((iInput<0)||(iInput>=input.mHeight)) continue;
            tempValue += (kernel(iKernel,jKernel)*input(iInput,jInput,k));
          }
        }

        result(iResult,jResult,k) = tempValue;
      }

      return result;
}

template<typename T> 
int medianFilter2DHelperFunc_Compare( const void *arg1, const void *arg2 )
{
  if((*(T*)arg1)<(*(T*)arg2)) return -1;
  else if ((*(T*)arg1)==(*(T*)arg2)) return 0;
  else return 1;
}

template<typename T> 
T medianFilter2DHelperFunc_FindMedian(int n, T*elements)
{
  qsort( (void *) elements, (size_t)n, sizeof(T), medianFilter2DHelperFunc_Compare<T>);

  if(n==0) return 0; 
  else if(n%2) /*odd*/
    return elements[(n-1)/2];
  else
    return elements[n/2];

}


/** 
* @relates ImageT
* Performs 2D median filtering on the input image, i.e., every output pixel is set to the median value within a rectangular block of pixels around it.
* 
* <p> e.g., 
* <pre> 
*   i2 = medianFilter2D(i1, 5, 5); 
* </pre>
* </p>
* @param input the input image
* @param filterHeight height of the median filter
* @param filterWidth width of the median filter
* @return filtered image
* @see minFilter2D(const ImageT<T> & input, int filterHeight, int filterWidth)
* @see maxFilter2D(const ImageT<T> & input, int filterHeight, int filterWidth)
* @see meanFilter2D(const ImageT<T> & input, int filterHeight, int filterWidth)
* @see filter2D(const ImageT<T> & kernel, const ImageT<T> & input)
* @see convolve2D(const ImageT<T> & kernel, const ImageT<T> & input)
*/
template<typename T> 
const ImageT<T> medianFilter2D (const ImageT<T> & input, int filterHeight, int filterWidth)
{
  int iResult,jResult,k,iKernel,jKernel,iInput,jInput,iKernelMidpoint,jKernelMidpoint;
  T*listOfElements, tempValue;
  int count;

  ImageT<T> result(input,false); //create same-sized copy without copying contents

  if(input.mSize==0) return result;
  if((filterHeight<1)||(filterWidth<1)) return result;

  listOfElements = new T[filterHeight*filterWidth];
  if(listOfElements==0) return result; //no memory

  iKernelMidpoint = filterHeight/2;
  jKernelMidpoint = filterWidth/2;

  for(k=0;k<result.mChannels;k++)
    for(jResult=0;jResult<result.mWidth;jResult++)
      for(iResult=0;iResult<result.mHeight;iResult++)
      {
        count = 0;
        for(jKernel=0; jKernel<filterWidth; jKernel++)
        {
          jInput = jResult + (jKernel-jKernelMidpoint); 
          if((jInput<0)||(jInput>=input.mWidth)) continue;

          for(iKernel=0; iKernel<filterHeight; iKernel++)
          {
            iInput = iResult + (iKernel-iKernelMidpoint); 
            if((iInput<0)||(iInput>=input.mHeight)) continue;

            tempValue = input(iInput,jInput,k);
            if(tempValue!=tempValue) continue; //check if value is NaN
            listOfElements[count] = tempValue;
            count++;						
          }
        }				

        result(iResult,jResult,k) = medianFilter2DHelperFunc_FindMedian<T>(count, listOfElements);
      }


      if(listOfElements!=0) delete [] listOfElements;
      return result;
}

/** 
* @relates ImageT
* Performs 2D minimum filtering on the input image, i.e., every output pixel is set to the minimum value within a rectangular block of pixels around it.
* 
* <p> e.g., 
* <pre> 
*   i2 = minFilter2D(i1, 5, 5); 
* </pre>
* </p>
* @param input the input image
* @param filterHeight height of the filter
* @param filterWidth width of the filter
* @return filtered image
* @see medianFilter2D(const ImageT<T> & input, int filterHeight, int filterWidth)
* @see maxFilter2D(const ImageT<T> & input, int filterHeight, int filterWidth)
* @see meanFilter2D(const ImageT<T> & input, int filterHeight, int filterWidth)
* @see filter2D(const ImageT<T> & kernel, const ImageT<T> & input)
* @see convolve2D(const ImageT<T> & kernel, const ImageT<T> & input)
*/
template<typename T> 
const ImageT<T> minFilter2D (const ImageT<T> & input, int filterHeight, int filterWidth)
{
  int iResult,jResult,k,iKernel,jKernel,iInput,jInput,iKernelMidpoint,jKernelMidpoint;
  T resultValue, tempValue;

  ImageT<T> result(input,false); //create same-sized copy without copying contents

  if(input.mSize==0) return result;
  if((filterHeight<1)||(filterWidth<1)) return result;

  iKernelMidpoint = filterHeight/2;
  jKernelMidpoint = filterWidth/2;

  for(k=0;k<result.mChannels;k++)
    for(jResult=0;jResult<result.mWidth;jResult++)
      for(iResult=0;iResult<result.mHeight;iResult++)
      {
        resultValue = input(iResult,jResult,k);

        for(jKernel=0; jKernel<filterWidth; jKernel++)
        {
          jInput = jResult + (jKernel-jKernelMidpoint); 
          if((jInput<0)||(jInput>=input.mWidth)) continue;

          for(iKernel=0; iKernel<filterHeight; iKernel++)
          {
            iInput = iResult + (iKernel-iKernelMidpoint); 
            if((iInput<0)||(iInput>=input.mHeight)) continue;

            tempValue = input(iInput,jInput,k);
            if(tempValue!=tempValue) continue; //check if value is NaN

            if(resultValue>tempValue) resultValue = tempValue;
          }
        }				

        result(iResult,jResult,k) = resultValue;
      }

      return result;
}


/** 
* @relates ImageT
* Performs 2D maximum filtering on the input image, i.e., every output pixel is set to the maximum value within a rectangular block of pixels around it.
* 
* <p> e.g., 
* <pre> 
*   i2 = maxFilter2D(i1, 5, 5); 
* </pre>
* </p>
* @param input the input image
* @param filterHeight height of the filter
* @param filterWidth width of the filter
* @return filtered image
* @see medianFilter2D(const ImageT<T> & input, int filterHeight, int filterWidth)
* @see minFilter2D(const ImageT<T> & input, int filterHeight, int filterWidth)
* @see meanFilter2D(const ImageT<T> & input, int filterHeight, int filterWidth)
* @see filter2D(const ImageT<T> & kernel, const ImageT<T> & input)
* @see convolve2D(const ImageT<T> & kernel, const ImageT<T> & input)
*/
template<typename T> 
const ImageT<T> maxFilter2D (const ImageT<T> & input, int filterHeight, int filterWidth)
{
  int iResult,jResult,k,iKernel,jKernel,iInput,jInput,iKernelMidpoint,jKernelMidpoint;
  T resultValue, tempValue;

  ImageT<T> result(input,false); //create same-sized copy without copying contents

  if(input.mSize==0) return result;
  if((filterHeight<1)||(filterWidth<1)) return result;

  iKernelMidpoint = filterHeight/2;
  jKernelMidpoint = filterWidth/2;

  for(k=0;k<result.mChannels;k++)
    for(jResult=0;jResult<result.mWidth;jResult++)
      for(iResult=0;iResult<result.mHeight;iResult++)
      {
        resultValue = input(iResult,jResult,k);

        for(jKernel=0; jKernel<filterWidth; jKernel++)
        {
          jInput = jResult + (jKernel-jKernelMidpoint); 
          if((jInput<0)||(jInput>=input.mWidth)) continue;

          for(iKernel=0; iKernel<filterHeight; iKernel++)
          {
            iInput = iResult + (iKernel-iKernelMidpoint); 
            if((iInput<0)||(iInput>=input.mHeight)) continue;

            tempValue = input(iInput,jInput,k);
            if(tempValue!=tempValue) continue; //check if value is NaN

            if(resultValue<tempValue) resultValue = tempValue;
          }
        }				

        result(iResult,jResult,k) = resultValue;
      }

      return result;
}

/** 
* @relates ImageT
* Performs 2D mean filtering on the input image, i.e., every output pixel is set to the mean value of a rectangular block of pixels around it.
* 
* <p> e.g., 
* <pre> 
*   i2 = meanFilter2D(i1, 5, 5); 
* </pre>
* </p>
* @param input the input image
* @param filterHeight height of the filter
* @param filterWidth width of the filter
* @return filtered image
* @see medianFilter2D(const ImageT<T> & input, int filterHeight, int filterWidth)
* @see minFilter2D(const ImageT<T> & input, int filterHeight, int filterWidth)
* @see maxFilter2D(const ImageT<T> & input, int filterHeight, int filterWidth)
* @see filter2D(const ImageT<T> & kernel, const ImageT<T> & input)
* @see convolve2D(const ImageT<T> & kernel, const ImageT<T> & input)
*/
template<typename T> 
const ImageT<T> meanFilter2D (const ImageT<T> & input, int filterHeight, int filterWidth)
{
  ImageT<T> result; 
  ImageT<T> kernel;

  if(input.mSize==0) return result;
  if((filterHeight<1)||(filterWidth<1)) return result;

  kernel.resetDimensions(filterHeight, filterWidth);
  kernel = 1;
  kernel /= kernel.L1Norm();

  result = filter2D(kernel,input);

  return result;
}

/** 
* @relates ImageT
* Computes mean of image pixels along a particular dimension [1 = along rows, 2 = along columns, 3 = along color channels (default)]
* 
* <p> e.g., 
* <br> i2 = mean(i1,3); 
* <br> Here, i2 has the same height and width as i1, but only one color channel (since we have averaged all color channels together)
* <br>
* <br> i2 = mean(i1,1); 
* <br> Here, i2 has the same width and channels as i1, but only one row (since we have averaged all rows together)
* </p>
* @param input the input image
* @param dimension [1 = along rows, 2 = along columns, 3 = along color channels (default)]
* @return image
*/
template<typename T> 
const ImageT<T> mean(const ImageT<T> & input, int dimension = 3)
{
  ImageT<T> result;
  int i,j,k;
  T tempVal;

  if((dimension<1)||(dimension>3)) dimension = 3;
  switch(dimension)
  {
  case 1:
    result.resetDimensions(1, input.mWidth, input.mChannels);
    for(k=0; k<input.mChannels;k++)
      for(j=0; j<input.mWidth;j++)
      {
        tempVal = 0;
        for(i=0; i<input.mHeight;i++) tempVal+=input(i,j,k);
        result(0,j,k) = tempVal/input.mHeight;
      }					
      break;
  case 2:
    result.resetDimensions(input.mHeight, 1, input.mChannels);
    for(k=0; k<input.mChannels;k++)
      for(i=0; i<input.mHeight;i++)
      {
        tempVal = 0;
        for(j=0; j<input.mWidth;j++) tempVal+=input(i,j,k);
        result(i,0,k) = tempVal/input.mWidth;
      }					
      break;
  case 3:
    result.resetDimensions(input.mHeight, input.mWidth, 1);
    for(j=0; j<input.mWidth;j++)
      for(i=0; i<input.mHeight;i++)
      {
        tempVal = 0;
        for(k=0; k<input.mChannels;k++) tempVal+=input(i,j,k);
        result(i,j,0) = tempVal/input.mChannels;
      }					
      break;
  }
  return result;
}

/** 
* @relates ImageT
* Computes minimum of image pixels along a particular dimension [1 = along rows, 2 = along columns, 3 = along color channels (default)]
* 
* <p> e.g., 
* <br> i2 = min(i1,3); 
* <br> Here, i2 has the same height and width as i1, but only one color channel 
* <br>
* <br> i2 = min(i1,1); 
* <br> Here, i2 has the same width and channels as i1, but only one row 
* </p>
* @param input the input image
* @param dimension [1 = along rows, 2 = along columns, 3 = along color channels (default)]
* @return image
*/
template<typename T> 
const ImageT<T> min(const ImageT<T> & input, int dimension = 3)
{
  ImageT<T> result;
  int i,j,k;
  T tempVal;

  if((dimension<1)||(dimension>3)) dimension = 3;
  switch(dimension)
  {
  case 1:
    result.resetDimensions(1, input.mWidth, input.mChannels);
    for(k=0; k<input.mChannels;k++)
      for(j=0; j<input.mWidth;j++)
      {
        tempVal = input(0,j,k);
        for(i=1; i<input.mHeight;i++) tempVal=(tempVal<input(i,j,k))?tempVal:input(i,j,k);
        result(0,j,k) = tempVal;
      }					
      break;
  case 2:
    result.resetDimensions(input.mHeight, 1, input.mChannels);
    for(k=0; k<input.mChannels;k++)
      for(i=0; i<input.mHeight;i++)
      {
        tempVal = input(i,0,k);
        for(j=1; j<input.mWidth;j++) tempVal=(tempVal<input(i,j,k))?tempVal:input(i,j,k);
        result(i,0,k) = tempVal;
      }					
      break;
  case 3:
    result.resetDimensions(input.mHeight, input.mWidth, 1);
    for(j=0; j<input.mWidth;j++)
      for(i=0; i<input.mHeight;i++)
      {
        tempVal = input(i,j,0);
        for(k=1; k<input.mChannels;k++) tempVal=(tempVal<input(i,j,k))?tempVal:input(i,j,k);
        result(i,j,0) = tempVal;
      }					
      break;
  }
  return result;
}

/** 
* @relates ImageT
* Computes maximum of image pixels along a particular dimension [1 = along rows, 2 = along columns, 3 = along color channels (default)]
* 
* <p> e.g., 
* <br> i2 = max(i1,3); 
* <br> Here, i2 has the same height and width as i1, but only one color channel 
* <br>
* <br> i2 = max(i1,1); 
* <br> Here, i2 has the same width and channels as i1, but only one row 
* </p>
* @param input the input image
* @param dimension [1 = along rows, 2 = along columns, 3 = along color channels (default)]
* @return image
*/
template<typename T> 
const ImageT<T> max(const ImageT<T> & input, int dimension = 3)
{
  ImageT<T> result;
  int i,j,k;
  T tempVal;

  if((dimension<1)||(dimension>3)) dimension = 3;
  switch(dimension)
  {
  case 1:
    result.resetDimensions(1, input.mWidth, input.mChannels);
    for(k=0; k<input.mChannels;k++)
      for(j=0; j<input.mWidth;j++)
      {
        tempVal = input(0,j,k);
        for(i=1; i<input.mHeight;i++) tempVal=(tempVal>input(i,j,k))?tempVal:input(i,j,k);
        result(0,j,k) = tempVal;
      }					
      break;
  case 2:
    result.resetDimensions(input.mHeight, 1, input.mChannels);
    for(k=0; k<input.mChannels;k++)
      for(i=0; i<input.mHeight;i++)
      {
        tempVal = input(i,0,k);
        for(j=1; j<input.mWidth;j++) tempVal=(tempVal>input(i,j,k))?tempVal:input(i,j,k);
        result(i,0,k) = tempVal;
      }					
      break;
  case 3:
    result.resetDimensions(input.mHeight, input.mWidth, 1);
    for(j=0; j<input.mWidth;j++)
      for(i=0; i<input.mHeight;i++)
      {
        tempVal = input(i,j,0);
        for(k=1; k<input.mChannels;k++) tempVal=(tempVal>input(i,j,k))?tempVal:input(i,j,k);
        result(i,j,0) = tempVal;
      }					
      break;
  }
  return result;
}


/** 
* @relates ImageT
* Sums image pixels along a particular dimension (1 = sum rows, 2 = sum columns, 3 = sum channels)
* 
* <p> e.g., 
* <br> i2 = sum(i1,3); 
* <br> Here, i2 has the same height and width as i1, but only one color channel (since we have summed all color channels together)
* <br>
* <br> i2 = sum(i1,1); 
* <br> Here, i2 has the same width and channels as i1, but only one row (since we have summed all rows together)
* </p>
* @param input the input image
* @param dimension the dimension to sum along [1 = sum rows, 2 = sum columns, 3 = sum channels(default)]
* @return summed image
*/
template<typename T> 
const ImageT<T> sum(const ImageT<T> & input, int dimension = 3)
{
  ImageT<T> result;
  int i,j,k;
  T tempVal;

  if((dimension<1)||(dimension>3)) dimension = 3;
  switch(dimension)
  {
  case 1:
    result.resetDimensions(1, input.mWidth, input.mChannels);
    for(k=0; k<input.mChannels;k++)
      for(j=0; j<input.mWidth;j++)
      {
        tempVal = 0;
        for(i=0; i<input.mHeight;i++) tempVal+=input(i,j,k);
        result(0,j,k) = tempVal;
      }					
      break;
  case 2:
    result.resetDimensions(input.mHeight, 1, input.mChannels);
    for(k=0; k<input.mChannels;k++)
      for(i=0; i<input.mHeight;i++)
      {
        tempVal = 0;
        for(j=0; j<input.mWidth;j++) tempVal+=input(i,j,k);
        result(i,0,k) = tempVal;
      }					
      break;
  case 3:
    result.resetDimensions(input.mHeight, input.mWidth, 1);
    for(j=0; j<input.mWidth;j++)
      for(i=0; i<input.mHeight;i++)
      {
        tempVal = 0;
        for(k=0; k<input.mChannels;k++) tempVal+=input(i,j,k);
        result(i,j,0) = tempVal;
      }					
      break;
  }
  return result;
}

/** 
* @relates ImageT
* Sums image pixels in a rectangular region.
* e.g.,
* <pre>
*    i2 = sumRegion(i1,10,20,5,30,0,1); 
* </pre>
* @param input the input image
* @param rowLo starting row (if omitted or set to a negative value, the default value of 0 is used)
* @param rowHi ending row (if omitted or set to a negative value, the default value used is (height-1) )
* @param columnLo starting column (if omitted or set to a negative value, the default value of 0 is used)
* @param columnHi ending column (if omitted or set to a negative value, the default value used is (width-1))
* @param channelLo starting channel (if omitted or set to a negative value, the default value of 0 is used)
* @param channelHi ending channel (if omitted or set to a negative value, the default value used is (number of channels-1))
* @return sum
*/
template<typename T>
T sumRegion(const ImageT<T> & input, int rowLo=-1, int rowHi=-1, int columnLo=-1, int columnHi=-1, int channelLo=-1, int channelHi=-1)
{
  int i,j,k;
  T result;

  if(rowLo<0) rowLo = 0; if(rowLo>=input.mHeight) rowLo = input.mHeight-1;
  if(rowHi<0) rowHi = input.mHeight-1; if(rowHi>=input.mHeight) rowHi = input.mHeight-1;	
  if(columnLo<0) columnLo = 0; if(columnLo>=input.mWidth) columnLo = input.mWidth-1;
  if(columnHi<0) columnHi = input.mWidth-1; if(columnHi>=input.mWidth) columnHi = input.mWidth-1;	
  if(channelLo<0) channelLo = 0; if(channelLo>=input.mChannels) channelLo = input.mChannels-1;
  if(channelHi<0) channelHi = input.mChannels-1; if(channelHi>=input.mChannels) channelHi = input.mChannels-1;	

  result = 0;

  for(k=channelLo;k<=channelHi;k++)
    for(j=columnLo;j<=columnHi;j++)
      for(i=rowLo;i<=rowHi;i++)
        result += input(i,j,k);

  return result;
}

/** 
* @relates ImageT
* Sums image pixels in a single channel.
* e.g.,
* <pre>
*    i2 = sumSingleChannel(i1,2);  //sums channel 2
* </pre>
* @param input the input image
* @param channel the channel to be summed
* @return sum
*/
template<typename T>
T sumSingleChannel(const ImageT<T> & input, int channel)
{
  if((channel<0)||(channel>=input.mChannels)) return 0;
  return sumRegion(input,-1,-1,-1,-1,channel,channel);
}

/** 
* @relates ImageT
* Sums all image pixels.
* e.g.,
* <pre>
*    i2 = sumAll(i1);  
* </pre>
* @return sum
*/
template<typename T>
T sumAll(const ImageT<T> & input)
{
  return sumRegion(input,-1,-1,-1,-1,-1,-1);
}

/** 
* @relates ImageT
* Find sum of absolute values of all pixels.
* e.g.,
* <pre>
*    temp = L1Norm(i1);  
* </pre>
* @param input the input image
* @return L1 norm
*/
template<typename T>
T L1Norm(const ImageT<T> & input)
{
  T result = 0;
  for(int i=0; i<input.mSize; i++) result += (T) fabs(input.mData[i]);
  return result;
}

/** 
* @relates ImageT
* Find sqrt of sum of squared pixel values.
* e.g.,
* <pre>
*    temp = L2Norm(i1);  
* </pre>
* @param input the input image
* @return L2 norm
*/
template<typename T>
T L2Norm(const ImageT<T> & input)
{
  T result = 0;
  for(int i=0; i<input.mSize; i++) result += (input.mData[i]*input.mData[i]);
  result = sqrt(result);
  return result;
}


/** 
* @relates ImageT
* Transposes the image. Each color channel is independently transposed.
* 
* <p> e.g., 
* <br> i2 = transpose(i1); 
* </p>
* @param input the input image
* @return transposed image
*/
template<typename T> 
const ImageT<T> transpose(const ImageT<T> & input)
{
  ImageT<T> result;
  int i,j,k;

  result.resetDimensions(input.mWidth, input.mHeight, input.mChannels); //transpose dimensions

  //fill transpose
  for(k=0; k<result.mChannels;k++)
    for(j=0; j<result.mWidth;j++)
      for(i=0; i<result.mHeight;i++)
      {
        result(i,j,k) = input(j,i,k);
      }

      return result;

}

/** 
* @relates ImageT
* Flips image left-to-right (i.e., about a vertical axis).
* 
* <p> e.g., 
* <br> i2 = flipLR(i1); 
* </p>
* @param input the input image
* @return flipped image
*/
template<typename T> 
const ImageT<T> flipLR(const ImageT<T> & input)
{
  ImageT<T> result(input,false);
  int i,j,k;

  //fill transpose
  for(k=0; k<result.mChannels;k++)
    for(j=0; j<result.mWidth;j++)
      for(i=0; i<result.mHeight;i++)
      {
        result(i,j,k) = input(i,(result.mWidth-1)-j,k);
      }

      return result;	
}

/** 
* @relates ImageT
* Flips image upside-down (i.e., about a horizontal axis).
* 
* <p> e.g., 
* <br> i2 = flipUD(i1); 
* </p>
* @param input the input image
* @return flipped image
*/
template<typename T> 
const ImageT<T> flipUD(const ImageT<T> & input)
{
  ImageT<T> result(input,false);
  int i,j,k;

  //fill transpose
  for(k=0; k<result.mChannels;k++)
    for(j=0; j<result.mWidth;j++)
      for(i=0; i<result.mHeight;i++)
      {
        result(i,j,k) = input((result.mHeight-1)-i,j,k);
      }

      return result;	
}

/** 
* @relates ImageT
* Converts multi-channel color image to single-channel gray image by averaging channels.
* <p> e.g., 
* <br> i2 = rgb2gray(i1); 
* </p>
* @param input the input image
* @return gray image
*/
template<typename T> 
const ImageT<T> rgb2gray(const ImageT<T> & input)
{
  ImageT<T> result(input,false);

  result = mean(input,3);	

  return result;	
}


/** 
* Converts multi-channel color image to single-channel gray image by averaging channels.
* <p> e.g., 
* <br> i1.setToGray(); 
* </p>
*/
template<typename T>
void ImageT<T>::setToGray()
{
  (*this) = rgb2gray(*this);
}

/** 
* @relates ImageT
* Tiles input image <b>height</b> times along rows, <b>width</b> times along columns and <b>channels</b> times along color channels.
* <p> e.g., 
* <br> i2 = repmat(i1,2,2,1);
* <br> Here, i2 is twice as large as i1, and is basically a 2x2 tiled copy of i1.
* </p>
* @param input the input image
* @param height number of vertical tiles (default = 1)
* @param width number of horizontal tiles (default = 1)
* @param channels number of color channel tiles (default = 1)
* @return image
*/
template<typename T> 
const ImageT<T> repmat (const ImageT<T> & input, int height=1, int width=1, int channels=1)
{
  ImageT<T> result;
  int i,j,k;

  result.resetDimensions(input.mHeight*height, input.mWidth*width, input.mChannels*channels); 

  for(k=0; k<result.mChannels;k++)
    for(j=0; j<result.mWidth;j++)
      for(i=0; i<result.mHeight;i++)
      {
        result(i,j,k) = input(i%input.mHeight,j%input.mWidth,k%input.mChannels);
      }

      return result;
}

/** 
* @relates ImageT
* Creates a translated copy of an image.
* <p> e.g., 
* <br> i2 = shiftImageXY(i1,10,20);
* <br> Here, i2 is i1 shifted vertically to the right by 10 pixels and down by 20 pixels.
* <br> Note: i1 and i2 have the same dimensions.
* </p>
* @param input the input image
* @param columns horizontal translation in pixels (default = 0)
* @param rows vertical translation in pixels (default = 0)
* @return translated image
*/
template<typename T> 
const ImageT<T> shiftImageXY (const ImageT<T> & input, int columns=0, int rows=0)
{	
  ImageT<T> result(input,false);
  int i,j,k, iLow, iHigh, jLow, jHigh;

  if(rows>=0) {iLow = rows; iHigh = result.mHeight;}
  else {iLow = 0; iHigh = result.mHeight + rows;}

  if(columns>=0) {jLow = columns; jHigh = result.mWidth;}
  else {jLow = 0; jHigh = result.mWidth + columns;}

  for(k=0; k<result.mChannels;k++)
    for(j=jLow; j<jHigh;j++)
      for(i=iLow; i<iHigh;i++)
      {
        result(i,j,k) = input(i-rows,j-columns,k); //translate
      }

      return result;	
}

/** 
* @relates ImageT
* Rescales the input image by a certain scale using the nearest-neighbor method and optional pre-smoothing.
* <p> e.g., 
* <br> i2 = resizeNearestNbr(i1, 0.5, true);
* <br> Here, i2 is half the size of i1 and presmoothing is set to true.
* <br> Pre-smoothing performs appropriate gaussian smoothing before rescaling and gives better results but is slower.
* </p>
* @param input the input image
* @param scale the magnification factor (in the range 0.01 to 100)
* @param preSmooth if true performs gaussian pre-smoothing. if false (default), no smoothing is performed.
* @return image
* @see resizeBilinear(const ImageT<T> & input, double scale, bool preSmooth)
*/
template<typename T> 
const ImageT<T> resizeNearestNbr(const ImageT<T> & input, double scale, bool preSmooth=false)
{
  int i,j,k;
  ImageT<T> result, intermediate, kernel;
  if((scale<0.01)||(scale>100)) return result; //enforce scaling limits: pretty large limits :)

  result.resetDimensions((int)floor(input.mHeight*scale),(int)floor(input.mWidth*scale),input.mChannels);

  if(preSmooth && (scale<1)) //smooth if presmooth is set and we are shrinking only; no need to smooth when magnifying image
  {
    kernel.setToGaussian((int)ceil((1/scale)*3),1/scale);
    intermediate = filter2D(kernel,input);		

    for(k=0; k<result.mChannels;k++)
      for(j=0; j<result.mWidth;j++)
        for(i=0; i<result.mHeight;i++)
        {
          result(i,j,k) = (T) intermediate(ov_round(i/scale),ov_round(j/scale),k); //scale
        }		
  }
  else
  {
    for(k=0; k<result.mChannels;k++)
      for(j=0; j<result.mWidth;j++)
        for(i=0; i<result.mHeight;i++)
        {
          result(i,j,k) = (T) input(ov_round(i/scale),ov_round(j/scale),k); //scale
        }		
  }

  return result;
}

/** 
* @relates ImageT
* Rescales the input image by a certain scale using bilinear interpolation and optional pre-smoothing.
* <p> e.g., 
* <br> i2 = resizeBilinear(i1, 0.5, true);
* <br> Here, i2 is half the size of i1 and presmoothing is set to true.
* <br> Pre-smoothing performs appropriate gaussian smoothing before rescaling and gives better results but is slower.
* </p>
* @param input the input image
* @param scale the magnification factor (in the range 0.01 to 100)
* @param preSmooth if true performs gaussian pre-smoothing. if false (default), no smoothing is performed.
* @return image
* @see resizeNearestNbr(const ImageT<T> & input, double scale, bool preSmooth)
*/
template<typename T> 
const ImageT<T> resizeBilinear(const ImageT<T> & input, double scale, bool preSmooth=false)
{	
  int i,j,k;
  double iInput,jInput;
  int iInputLo,jInputLo,iInputHi,jInputHi;
  T interp1, interp2;

  ImageT<T> result, intermediate, kernel;
  if((scale<0.01)||(scale>100)) return result; //enforce scaling limits: pretty large limits :)

  result.resetDimensions((int)floor(input.mHeight*scale),(int)floor(input.mWidth*scale),input.mChannels);

  if(preSmooth && (scale<1)) //smooth if presmooth is set and we are shrinking only; no need to smooth when magnifying image
  {
    kernel.setToGaussian((int)ceil((1/scale)*3),1/scale);
    intermediate = filter2D(kernel,input);		

    for(k=0; k<result.mChannels;k++)
      for(j=0; j<result.mWidth;j++)
        for(i=0; i<result.mHeight;i++)
        {
          iInput = i/scale; iInputLo = (int)iInput; iInputHi = (int)(iInput+1);
          jInput = j/scale; jInputLo = (int)jInput; jInputHi = (int)(jInput+1);

          interp1 = (T) ((iInputHi-iInput)*intermediate(iInputLo,jInputLo,k) + (iInput-iInputLo)*intermediate(iInputHi,jInputLo,k));
          interp2 = (T) ((iInputHi-iInput)*intermediate(iInputLo,jInputHi,k) + (iInput-iInputLo)*intermediate(iInputHi,jInputHi,k));

          result(i,j,k) = (T) ((jInputHi-jInput)*interp1+(jInput-jInputLo)*interp2);
        }		
  }
  else
  {
    for(k=0; k<result.mChannels;k++)
      for(j=0; j<result.mWidth;j++)
        for(i=0; i<result.mHeight;i++)
        {
          iInput = i/scale; iInputLo = (int)iInput; iInputHi = (int)(iInput+1);
          jInput = j/scale; jInputLo = (int)jInput; jInputHi = (int)(jInput+1);

          interp1 = (T) ((iInputHi-iInput)*input(iInputLo,jInputLo,k) + (iInput-iInputLo)*input(iInputHi,jInputLo,k));
          interp2 = (T) ((iInputHi-iInput)*input(iInputLo,jInputHi,k) + (iInput-iInputLo)*input(iInputHi,jInputHi,k));

          result(i,j,k) = (T) ((jInputHi-jInput)*interp1+(jInput-jInputLo)*interp2);
        }		
  }

  return result;
}

/**
* Sets image contents to random values ranging from lowerbound to upperbound (inclusive).
* e.g.,
* <pre>
*   i1.setToRandom(10,20);
* </pre>
* @param lowerbound lower bound on the desired random numbers
* @param upperbound upper bound on the desired random numbers
* @see #random(double lowerbound, double upperbound, int height, int width, int nColorChannels)
*/
template<typename T>
void ImageT<T>::setToRandom(double lowerbound, double upperbound)
{
  for(int i=0; i<mSize; i++) mData[i] = (double) (rand()*(upperbound-lowerbound))/RAND_MAX + lowerbound;
}

/**
* Set caller to an image of height y2-y1+1 and width x2-x1+1 with each pixel set to its x-coordinate (from x1 to x2).
* e.g.,
* <pre>
*   i1.setToMeshgridX(1,5,1,4,2,1); //returns image of height 4, width 3, with each row equal to [1 3 5]
*      OR using default step sizes
*   i1.setToMeshgridX(5,8,1,12); 
* </pre>
*
* @param x1 starting x-value 
* @param x2 ending x-value
* @param y1 starting y-value
* @param y2 ending y-value
* @param dx stepsize along x (if omitted, default = 1)
* @param dy stepsize along y (if omitted, default = 1)
* @see setToMeshgridY(T x1, T x2, T y1, T y2, T dx, T dy)
* @see #meshgridX(double x1, double x2, double y1, double y2, double dx, double dy)
* @see #meshgridY(double x1, double x2, double y1, double y2, double dx, double dy)
*/
template<typename T> 
void ImageT<T> ::setToMeshgridX (T x1, T x2, T y1, T y2, T dx, T dy)
{
  int i,j,height,width;

  width = 1 + (int) floor((double)(x2-x1)/dx);
  height  = 1 + (int) floor((double)(y2-y1)/dy);

  printf("Yes: %d,%d\n", height,width);

  this->resetDimensions(height, width, 1); 

  for(j=0; j<width; j++)
    for(i=0; i<height; i++)
    {
      (*this)(i,j) = x1 + j*dx;
    }
}

/**
* Set caller to an image of height y2-y1+1 and width x2-x1+1 with each pixel set to its x-coordinate (from x1 to x2).
* e.g.,
* <pre>
*   i1.setToMeshgridY(1,5,1,4,2,1); //returns image of height 4, width 3, with each column equal to [1 2 3 4]
*      OR using default step sizes
*   i1.setToMeshgridY(5,8,7,12); 
* </pre>
*
* @param x1 starting x-value 
* @param x2 ending x-value
* @param y1 starting y-value
* @param y2 ending y-value
* @param dx stepsize along x (if omitted, default = 1)
* @param dy stepsize along y (if omitted, default = 1)
* @see setToMeshgridX(T x1, T x2, T y1, T y2, T dx, T dy)
* @see #meshgridX(double x1, double x2, double y1, double y2, double dx, double dy)
* @see #meshgridY(double x1, double x2, double y1, double y2, double dx, double dy)
*/
template<typename T> 
void ImageT<T> ::setToMeshgridY (T x1, T x2, T y1, T y2, T dx, T dy)
{
  int i,j,height,width;

  width = 1 + (int) floor((double)(x2-x1)/dx);
  height  = 1 + (int) floor((double)(y2-y1)/dy);

  printf("Yes: %f,%f,%f\n", x1,x2,dx);

  this->resetDimensions(height, width, 1); 

  for(j=0; j<mWidth; j++)
    for(i=0; i<mHeight; i++)
    {
      (*this)(i,j) = y1 + i*dy;
    }
}

/**
* Sets the image to gaussian of half-width sigma and height and width equal to size.
* e.g.,
* <pre>
*    i1.setToGaussian(10,3);
* </pre>
* @param size height and width are both set equal to size
* @param sigma halfwidth of the gaussian
* @see gaussian(int size, double sigma)
* @see ImageT<T>#setToGaborX(int size, double sigma, double period, double phaseshift)
* @see ImageT<T>#setToGaborY(int size, double sigma, double period, double phaseshift)
* @see #gaborX(int size, double sigma, double period, double phaseshift)
* @see #gaborY(int size, double sigma, double period, double phaseshift)
*/
template<typename T> 
void ImageT<T> ::setToGaussian(int size, double sigma)
{
  double x,y;
  double halfsize;

  this->resetDimensions(size, size, 1); 
  halfsize = (size-1)/2.0;	

  for(int j=0; j<mWidth; j++)
    for(int i=0; i<mHeight; i++)
    {
      x = j-halfsize;
      y = i-halfsize;
      (*this)(i,j) = (T) exp(-0.5*(x*x+y*y)/(sigma*sigma));
    }	
    (*this) /= L1Norm(*this);
}

/**
* Sets the image to a horizontal gabor filter.
* e.g.,
* <pre>
*    i1.setToGaborX(10,3,3,0);
* </pre>
* @param size height and width are both set equal to size
* @param sigma halfwidth of the gaussian envelope
* @param period the period of the sinusoid
* @param phaseshift (the phase of the sinusoid in degrees (default is 0)).
* @see #gaborX(int size, double sigma, double period, double phaseshift)
* @see ImageT<T>#setToGaborY(int size, double sigma, double period, double phaseshift)
* @see #gaborY(int size, double sigma, double period, double phaseshift)
* @see ImageT<T>#setToGaussian(int size, double sigma)
* @see gaussian(int size, double sigma)
*/
template<typename T> 
void ImageT<T> ::setToGaborX(int size, double sigma, double period, double phaseshift=0)
{
  double x,y;
  double halfsize, tempVal, normalizer;
  double pi = 2 * acos(0.0);

  if(size<=0) return;
  if(sigma<=0) return;
  if(period<=0) return;

  phaseshift = phaseshift*pi/180; //convert phaseshift from degrees to radians

  this->resetDimensions(size, size, 1); 
  halfsize = (size-1)/2.0;	

  normalizer = 0;
  for(int j=0; j<mWidth; j++)
    for(int i=0; i<mHeight; i++)
    {
      x = j-halfsize;
      y = i-halfsize;

      tempVal = exp(-0.5*(x*x+y*y)/(sigma*sigma)); //gaussian envelope
      normalizer += tempVal; //normalise using gaussian envelope only

      //now multiply by sinusoid to get gabor function
      (*this)(i,j) = (T) (tempVal*sin(2.0*pi*x/period + phaseshift));
    }	

    (*this) /= (T) normalizer;
}

/**
* Sets the image to a vertical gabor filter.
* e.g.,
* <pre>
*    i1.setToGaborY(10,3,3,0);
* </pre>
* @param size height and width are both set equal to size
* @param sigma halfwidth of the gaussian envelope
* @param period the period of the sinusoid
* @param phaseshift (the phase of the sinusoid in degrees (default is 0)).
* @see #gaborY(int size, double sigma, double period, double phaseshift)
* @see ImageT<T>#setToGaborX(int size, double sigma, double period, double phaseshift)
* @see #gaborX(int size, double sigma, double period, double phaseshift)
* @see ImageT<T>#setToGaussian(int size, double sigma)
* @see gaussian(int size, double sigma)
*/
template<typename T> 
void ImageT<T> ::setToGaborY(int size, double sigma, double period, double phaseshift=0)
{
  double x,y;
  double halfsize, tempVal, normalizer;
  double pi = 2 * acos(0.0);

  if(size<=0) return;
  if(sigma<=0) return;
  if(period<=0) return;

  phaseshift = phaseshift*pi/180; //convert phaseshift from degrees to radians

  this->resetDimensions(size, size, 1); 
  halfsize = (size-1)/2.0;	

  normalizer = 0;
  for(int j=0; j<mWidth; j++)
    for(int i=0; i<mHeight; i++)
    {
      x = j-halfsize;
      y = i-halfsize;

      tempVal = exp(-0.5*(x*x+y*y)/(sigma*sigma)); //gaussian envelope
      normalizer += tempVal; //normalise using gaussian envelope only

      //now multiply by sinusoid to get gabor function
      (*this)(i,j) = (T) (tempVal*sin(2.0*pi*y/period + phaseshift));
    }	

    (*this) /= (T) normalizer;
}

/** Creates an image filled with random numbers ranging from lowerbound to upperbound.
* <br>e.g., 
* <pre> 
*    i1 = random(20,30,3,3,1);
* </pre>
*
* @param lowerbound lower bound on the desired random numbers
* @param upperbound upper bound on the desired random numbers
* @param height	desired height of the image
* @param width desired width of image
* @param nColorChannels desired number of color channels
* @return a new image of type ImageT<double>
* @see ImageT<T>#setToRandom(double lowerbound, double upperbound)
*/

inline const ImageT<double> random(double lowerbound, double upperbound, int height, int width, int nColorChannels = 1)
{
  ImageT<double> result(height,width,nColorChannels);
  result.setToRandom(lowerbound, upperbound);
  return result;
}

/**
* Creates an image of height y2-y1+1 and width x2-x1+1 with each pixel set to its x-coordinate (from x1 to x2).
* e.g.,
* <pre>
*   i1 = meshgridX(1,5,1,4,2,1); //returns image of height 4, width 3, with each row equal to [1 3 5]
*      OR using default step sizes
*   i1 = meshgridX(5,8,1,12); 
* </pre>
*
* @param x1 starting x-value 
* @param x2 ending x-value
* @param y1 starting y-value
* @param y2 ending y-value
* @param dx stepsize along x (if omitted, default = 1)
* @param dy stepsize along y (if omitted, default = 1)
* @return an image of type ImageT<double>
* @see #meshgridY(double x1, double x2, double y1, double y2, double dx, double dy)
* @see ImageT<T>#setToMeshgridX(T x1, T x2, T y1, T y2, T dx, T dy)
* @see ImageT<T>#setToMeshgridY(T x1, T x2, T y1, T y2, T dx, T dy)
*/
inline const ImageT<double> meshgridX (double x1, double x2, double y1, double y2, double dx = 1, double dy = 1)
{
  ImageT<double> result;
  result.setToMeshgridX(x1, x2, y1, y2, dx, dy);
  return result;
}

/**
* Creates an image of height y2-y1+1 and width x2-x1+1 with each pixel set to its y-coordinate (from y1 to y2).
* e.g.,
* <pre>
*   i1 = meshgridY(1,5,1,4,2,1); //returns image of height 4, width 3, with each column equal to [1 2 3 4]
*      OR using default step sizes
*   i1 = meshgridY(5,8,1,12); 
* </pre>
*
* @param x1 starting x-value 
* @param x2 ending x-value
* @param y1 starting y-value
* @param y2 ending y-value
* @param dx stepsize along x (if omitted, default = 1)
* @param dy stepsize along y (if omitted, default = 1)
* @return an image of type ImageT<double>
* @see #meshgridX(double x1, double x2, double y1, double y2, double dx, double dy)
* @see ImageT<T>#setToMeshgridY(T x1, T x2, T y1, T y2, T dx, T dy)
* @see ImageT<T>#setToMeshgridX(T x1, T x2, T y1, T y2, T dx, T dy)
*/
inline const ImageT<double> meshgridY (double x1, double x2, double y1, double y2, double dx = 1, double dy = 1)
{
  ImageT<double> result;
  result.setToMeshgridY(x1, x2, y1, y2, dx, dy);
  return result;
}

/**
* Creates image equal to gaussian of half-width sigma and height and width equal to size.
* e.g.,
* <pre>
*    i1 = gaussian(10,3);
* </pre>
* @param size height and width are both set equal to size
* @param sigma halfwidth of the gaussian
* @return an image of type ImageT<double>
* @see ImageT<T>#setToGaussian(int size, double sigma)
* @see #gaborX(int size, double sigma, double period, double phaseshift)
* @see #gaborY(int size, double sigma, double period, double phaseshift)
* @see ImageT<T>#setToGaborX(int size, double sigma, double period, double phaseshift)
* @see ImageT<T>#setToGaborY(int size, double sigma, double period, double phaseshift)
*/
inline const ImageT<double> gaussian(int size, double sigma)
{
  ImageT<double> result;
  result.setToGaussian(size, sigma);
  return result;
}

/**
* Sets the image to a horizontal gabor filter.
* e.g.,
* <pre>
*    i1 = gaborX(10,3,3,0);
* </pre>
* @param size height and width are both set equal to size
* @param sigma halfwidth of the gaussian envelope
* @param period the period of the sinusoid
* @param phaseshift (the phase of the sinusoid in degrees (default is 0)).
* @see #gaborY(int size, double sigma, double period, double phaseshift)
* @see ImageT<T>#setToGaborX(int size, double sigma, double period, double phaseshift)
* @see ImageT<T>#setToGaborY(int size, double sigma, double period, double phaseshift)
* @see #gaussian(int size, double sigma)
* @see ImageT<T>#setToGaussian(int size, double sigma)
*/
inline const ImageT<double> gaborX(int size, double sigma, double period, double phaseshift)
{
  ImageT<double> result;
  result.setToGaborX(size, sigma, period, phaseshift);
  return result;
}

/**
* Sets the image to a vertical gabor filter.
* e.g.,
* <pre>
*    i1 = gaborY(10,3,3,0);
* </pre>
* @param size height and width are both set equal to size
* @param sigma halfwidth of the gaussian envelope
* @param period the period of the sinusoid
* @param phaseshift (the phase of the sinusoid in degrees (default is 0)).
* @return an image of type ImageT<double>
* @see #gaborX(int size, double sigma, double period, double phaseshift)
* @see ImageT<T>#setToGaborY(int size, double sigma, double period, double phaseshift)
* @see ImageT<T>#setToGaborX(int size, double sigma, double period, double phaseshift)
* @see #gaussian(int size, double sigma)
* @see ImageT<T>#setToGaussian(int size, double sigma)
*/
inline const ImageT<double> gaborY(int size, double sigma, double period, double phaseshift)
{
  ImageT<double> result;
  result.setToGaborY(size, sigma, period, phaseshift);
  return result;
}


/**
* Sets the image to a gabor filter with arbitrary orientation
* e.g. (to use a filter with 45 degree orientation),
* <pre>
*    i1.setToGaborOriented(10,3,3,45,0);
* </pre>
* @param size height and width are both set equal to size
* @param sigma halfwidth of the gaussian envelope
* @param period the period of the sinusoid
* @param angle the orientation of the sinusoid (degrees)
* @param phaseshift (the phase of the sinusoid in degrees (default is 0)).
* @see #gaborOriented(int size, double sigma, double period, double angle, double phaseshift)
* @see ImageT<T>#setToGaborX(int size, double sigma, double period, double phaseshift)
* @see #gaborX(int size, double sigma, double period, double phaseshift)
* @see ImageT<T>#setToGaborX(int size, double sigma, double period, double phaseshift)
* @see #gaborY(int size, double sigma, double period, double phaseshift)
*/
template<typename T> 
void ImageT<T>::setToGaborOriented(int size, double sigma, double period, double angle, double phaseshift=0)
{
  double x,y;
  double halfsize, tempVal, normalizer;
  double pi = 2 * acos(0.0);

  if(size<=0) return;
  if(sigma<=0) return;
  if(period<=0) return;

  phaseshift = phaseshift*pi/180; //convert phaseshift from degrees to radians
  angle      = angle*pi/180;		//convert orientation from degrees to radians

  this->resetDimensions(size, size, 1); 
  halfsize = (size-1)/2.0;	

  normalizer = 0;
  for(int j=0; j<mWidth; j++)
    for(int i=0; i<mHeight; i++)
    {
      x = j-halfsize;
      y = i-halfsize;

      tempVal = exp(-0.5*(x*x+y*y)/(sigma*sigma)); //gaussian envelope
      normalizer += tempVal; //normalise using gaussian envelope only

      //now multiply by sinusoid to get gabor function
      (*this)(i,j) = (T) (tempVal*sin(2.0*pi*(x*cos(angle)+y*sin(angle))/period + phaseshift));
    }	

    (*this) /= (T) normalizer;
}

/**
* Creates a gabor filter with arbitrary orientation
* e.g. (to use a filter with 45 degree orientation),
* <pre>
*    i1 = gaborOriented(10,3,3,45,0);
* </pre>
* @param size height and width are both set equal to size
* @param sigma halfwidth of the gaussian envelope
* @param period the period of the sinusoid
* @param angle the orientation of the sinusoid (degrees)
* @param phaseshift (the phase of the sinusoid in degrees (default is 0)).
* @see ImageT<T>#setGaborOriented(int size, double sigma, double period, double angle, double phaseshift)
* @see ImageT<T>#setToGaborX(int size, double sigma, double period, double phaseshift)
* @see #gaborX(int size, double sigma, double period, double phaseshift)
* @see ImageT<T>#setToGaborX(int size, double sigma, double period, double phaseshift)
* @see #gaborY(int size, double sigma, double period, double phaseshift)
*/
inline const ImageT<double> gaborOriented(int size, double sigma, double period, double angle, double phaseshift=0)
{
  ImageT<double> result;
  result.setToGaborOriented(size, sigma, period, angle, phaseshift);
  return result;
}

/**
* Run Gabor filters with 4 orientation and 4 scales, and return the phases
* at each filter stacked as a 16 channel image.
* <pre>
*    phases = i1.getGaborPhaseStack();
* </pre>
* @return an image with 16 channels with each channel containing the convolution with a gabor filter of certain orientation and scale.
* @see ImageT<T>#setGaborOriented(int size, double sigma, double period, double angle, double phaseshift)
* @see #gaborOriented(int size, double sigma, double period, double angle, double phaseshift)
* @see ImageT<T>#setToGaborX(int size, double sigma, double period, double phaseshift)
* @see #gaborX(int size, double sigma, double period, double phaseshift)
* @see ImageT<T>#setToGaussian(int size, double sigma)
* @see #gaborY(int size, double sigma, double period, double phaseshift)
* @see gaussian(int size, double sigma)
*/
template<typename T> 
ImageT<double> ImageT<T>::getGaborPhaseStack()
{
  using namespace std;

  ImageT<double> copyOfThis(*this,true);
  ImageT<double> resultA;
  ImageT<double> resultB;
  ImageT<double> phase;

  ImageT<double> filterA;
  ImageT<double> filterB;

  ImageT<double> result = ImageT(mHeight, mWidth, 16);

  int n = 0;
  for(double period=4; period<= 32; period*=2)
    for(double angle=0; angle<180; angle += 45){
      filterA.setToGaborOriented(31,2*period,period,angle,0);
      filterB.setToGaborOriented(31,2*period,period,angle,90);

      resultA = convolve2D(filterA,copyOfThis);
      resultB = convolve2D(filterB,copyOfThis);

      phase = atan2(resultA, resultB);

      result.copyChannel(phase,0,n);

      n++;
    }


    return result;
}

/**
* Returns true if the two input images have the same dimensions (height, width, number of channels).
* @param i1 first image
* @param i2 second image
* @return true (if same dimensions) or false (if different dimensions)
* @see checkEqualHeightWidth(const ImageT<T> & i1, const ImageT<T> & i2)
*/
template<typename T> 
bool haveEqualDimensions(const ImageT<T> & i1, const ImageT<T> & i2)
{
  return ((i1.mWidth==i2.mWidth)&&(i1.mHeight==i2.mHeight)&&(i1.mChannels==i2.mChannels));
}

/**
* Returns true if the two input images have the same height and width.
* Note: This ignores the number of channels, hence the images may have different number of channels.
* @param i1 first image
* @param i2 second image
* @return true or false 
* @see checkEqualDimensions(const ImageT<T> & i1, const ImageT<T> & i2)
*/
template<typename T> 
bool haveEqualHeightWidth(const ImageT<T> & i1, const ImageT<T> & i2)
{
  return ((i1.mWidth==i2.mWidth)&&(i1.mHeight==i2.mHeight));
}

template<>
const ImageT<bool> ImageT<bool>::operator ! () const
{
  ImageT<bool> result(*this); 

  for(int i=0; i<result.mSize; i++)
    result.mData[i] = !result.mData[i];

  return (result); 
}

const ImageT<bool> operator && (const ImageT<bool> & i1, const ImageT<bool> & i2)
{
  ImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

  if((i1.mHeight==i2.mHeight) && (i1.mWidth==i2.mWidth) && (i1.mChannels==i2.mChannels))
  {
    for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] && i2.mData[i]);
  }

  return result;
}


const ImageT<bool> operator || (const ImageT<bool> & i1, const ImageT<bool> & i2) 
{
  ImageT<bool> result(i1.mHeight,i1.mWidth,i1.mChannels);

  if((i1.mHeight==i2.mHeight) && (i1.mWidth==i2.mWidth) && (i1.mChannels==i2.mChannels))
  {
    for(int i=0; i<result.mSize;i++) result.mData[i] = (i1.mData[i] || i2.mData[i]);
  }

  return result;
}

}

#endif //__OVIMAGET_H

