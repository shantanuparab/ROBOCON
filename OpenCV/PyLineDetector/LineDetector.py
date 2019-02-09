# -*- coding: utf-8 -*-
"""
Created on Fri Jan 25 19:06:41 2019

@author: ChinmayTodankar
"""
import cv2;
import numpy;

class Properties:
    CLAHEClipLimit:float = 40;
    CLAHETileGrid: tuple = (15,15);
    #ROI = (0,0,0,0);
    def __init__(self):
        self.ColourBounds = [];
    def AddColourBounds(self, lower: numpy.array, higher: numpy.array):
        self.ColourBounds.append(lower, higher);

class Line:
    def __init__(self, p_property: Properties):
        self.Property : Properties = p_property;
        
    # Apply InRAngeFunction to extract Colours
    # From Given Image
    def ExtractColourBounds(self, src: numpy.array, dst: numpy.array):
        for lower,higher in self.Property.ColourBounds :
            # Extract Mask from Image Range
            mask:numpy.array = cv2.inRange(src, lower, higher);
            # Add Mask to Output Image
            dst = cv2.bitwise_or(dst, mask);
    
    # Return (Confirmation, Centroid)
    def CalculateContourCentroidByMoment(self, p_contour:numpy.array, binary : bool):
        moment = cv2.moments(p_contour, binary);
        if(moment['m00'] == 0):
            # The Point Returned in this scenario doesn't matter
            # As the False Performs Appropriate Task
            return (False, (0,0));
        
        #Calculate Centroid

        cx = int(moment['m10']/moment['m00']);
        cy = int(moment['m01']/moment['m00']);
        
        return (True,(cx, cy));

    def CorrectIllumination(self, img : numpy.array):
        # The following code relies on using the CLAHE Algorithm
        # We could use Adapative Histogram but it was found
        # That the Given Algorithm has issues related to accuracy.
        # Read more about CLAHE at
        # https://en.wikipedia.org/wiki/Adaptive_histogram_equalization#Contrast_Limited_AHE
    
        #Please see
        # https://stackoverflow.com/questions/24341114/simple-illumination-correction-in-images-opencv-c

        # Store the Image in YUV Format
        # YUV Stores Luminosity Component as well
        # In Y Part
        # We apply CLAHE to it

        # 3 as Split into 3, Y,U,V
        img = cv2.cvtColor(img, cv2.COLOR_BGR2YUV, 3);
        y,u,v = cv2.split(img);
        
        clahe = cv2.createCLAHE(clipLimit = self.Property.CLAHEClipLimit, tileGridSize = self.Property.CLAHETileGrid);
        clahe.apply(y);
        # Merge Illuminated Images into YUV
        img = cv2.merge([y,u,v]);

        # Return Illuminated Images with BGR
        return cv2.cvtColor(img, cv2.COLOR_YUV2BGR);
    
    def FindLargestContourByArea(self, img: numpy.array):
        image, contours, hierarchy =   cv2.findContours(img,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE);    
        
        if(len(contours) == 0):
            return (False,numpy.array(0));
        return max(contours, key = cv2.contourArea);

    def ProcessImage(self, src: numpy.array): 
        # Perform Dilation on Image First
        elem = cv2.getStructuringElement(cv2.MORPH_DILATE, numpy.ones((3,3),np.uint8));
        dilated = cv2.dilate(src, elem);