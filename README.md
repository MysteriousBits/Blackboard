# Blackboard
A very simple and light-weight drawing app made with qt and C++.  
It supports tablet and **pen pressure** with the help of `QTabletEvents`. So you can-  
        *Draw and write in the simple canvas of the app with pen(pressure sensitive stroke) or mouse*  
        *Change pen color*  
        *Change background color*  
        *Pen size, eraser, clearing board, other basic drawing functionalities.*  
        *Open image from disk and draw into it*  
        *Save your file in multiple image formats*  
        *Quickly switch tools with assigned shortcuts*  

**Crop and save:** You can resize the application window to crop extra parts of the image before saving ;P But resizing the window doesnt actually crop the image. So you will get the part back when you sacle the window up again!
  

![icon](https://user-images.githubusercontent.com/80115356/141755218-0f76e086-e1c2-4f89-ab3e-5b004f12d229.png)  
![ss](https://user-images.githubusercontent.com/80115356/141756973-5f2e6c5e-4c4d-4bb0-b28d-adc8dcdb8261.jpg)


Some limitations of these are, *one step undo support only :) and handling low res images*. So jagged lines may show up while using smaller brushes. But point interpolation function is implemented with a soft alpha texture. So you will get smoother lines with larger brush size. Another thing not implemented yet is zooming and panning.
But as it handle low res images and limited functionalities, it is very light in resources usage.  
  
It is a learning project only. So feel free to fork the repository, develop your own and share with me ofcourse!  
You can grab a standalone executable file for windows from [here](https://github.com/UnknownComplexity/Blackboard/releases/tag/v1.0).

### Building from souce
Download Qt6 from official web. Clone the repository. In Qt creator, you can just load the project and hit build. Or,
```
qmake BlackBoard.pro
make
```
