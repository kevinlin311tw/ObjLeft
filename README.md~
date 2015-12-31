# ObjLeft

Abandoned Object Detection via Temporal Consistency Modeling and Back-Tracing Verification for Visual Surveillance

Created by Kevin Lin, Shen-Chi Chen, Chu-Song Chen, Daw-Tung Lin, Yi-Ping Hung at National Taiwan University.

_FYI: This is an industry-oriented project. Most of the algorithms are implemented using C/C++. We use OpenCV only for video input and visualization._

## Introduction

This paper presents an effective approach for detecting abandoned luggage in surveillance videos. We combine short- and long-term background models to extract foreground objects, where each pixel in an input image is classified as a 2-bit code. Subsequently, we introduce a framework to identify static foreground regions based on the temporal transition of code patterns, and to determine whether the candidate regions contain abandoned objects by analyzing the back-traced trajectories of luggage owners. Experimental results show that our method performs more favorable against the others.

The details can be found in the following [IEEE TIFS 2015 paper](http://www.csie.ntu.edu.tw/~r01944012/TIFS15_LIN.pdf)


## Citing the detection works

If you find our works useful in your research, please consider citing:

    Abandoned Object Detection via Temporal Consistency Modeling and Back-Tracing Verification for Visual Surveillance
    K. Lin, S.-C. Chen, C.-S. Chen, D.-T. Lin, and Y.-P. Hung
    IEEE Transactions on Information Forensic and Security (TIFS), 2015  


    Left-Luggage Detection from Finite-State-Machine Analysis in Static-Camera Videos
    K. Lin, S.-C. Chen, C.-S. Chen, D.-T. Lin, and Y.-P. Hung
    International Conference on Pattern Recognition (ICPR), 2014 


## Prerequisites

  0. OpenCV (tested on 64-bit Linux)


## Setup ObjLeft

Simply run the following commands:

    $ cmake .
    $ make
    $ ./download_video.sh


## Demo

This demo detect abandoned luggage in the video

    $ ./ObjLeft

Select the input source (1: video, 2: camera)

    $ 1

Input the filename if you choose video

    $ pets2006_1.avi

Click a rectangular Region of Interest (ROI) for detection

<img src="https://www.csie.ntu.edu.tw/~r01944012/objleft_fig1.png" width="500">

Double-press any key and start detection

<img src="https://www.csie.ntu.edu.tw/~r01944012/objleft_fig2.png" width="800">

This demo will detect the abandoned luggage and the owner trajectory as shown below.


## Contact

Please feel free to leave suggestions or comments to Kevin Lin (kevinlin311.tw@iis.sinica.edu.tw)
