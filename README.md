sip 
===
## Small Image Processing Scripts in native C, using libjpeg 
---------------------------------------------------------

### Usage Instructions

#### **Install libjpeg:**
- [libjpeg](http://ijg.org/).
- [libjpeg git repo](https://github.com/LuaDist/libjpeg/).

#### **Commands:**
	
	$ gcc -o name_script file_name.c -ljpeg            //to create an executable script
	
	$ ./name_script input_image.jpg output_image.jpg   //to execute the scripts
  
#### **Example JPG Files**

Check out the example files converted using filters in the example_out folders.

### Contents 

#### convolution_filters

- Edge Detection Filters 
	* Perwitt
	* Robcross
	* Sobel
	* Laplace 
	* kirsch

- Blur filters
	* Mean Filter
	* Motion Blur
	* Gaussian

- Other
	* Sharpen
	* Emboss

#### gray_filters

- Average Method
- Weighted Method

#### median_filters

- Normal median filter

- Median Fast Algo
	* Pixel Sort
	* Quick Sort
	* Quick Select Sort
	* Torben Method
	* Wirth Method

#### open

- Histogram Equalisation

- Image Compare

- Invert Images RGB TO GRAY










