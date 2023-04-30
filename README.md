# Convex Bounding Polygon

## Task

The goal of this project is to implement an algorithm that computes the convex bounding polygon of a set of 2D points, and provide a user interface that allows the user to place points on a surface or delete them. Once the user is satisfied with the point set, they should be able to initiate the convex bounding polygon calculation, which would generate a polygonal shape that encloses all the points and has the smallest possible area. The resulting polygon should be drawn on the screen using a different color than the points. It should be possible to update the polygon, if the user wants to place additional points.

## Technicalities

I used Visual Studio (version Visual C++ 14.2) C++17 Standard and the wxWidgets library (version 3.2.2.1).

## Algorithm

The idea is to find the leftmost point in the point set, and then iterate through the points in a counter-clockwise manner by determining the next point based on the slope of the line connecting the current point to the next point.

We start by identifying the point that is located furthest to the left (or the bottom-most one, in case there are multiple points with the same minimum x value). Then we analyze the rest of the point set, and try to find the next point in the right direction. We connect each eligible point with the start point and determine the slope values for these lines. If there is only one minimum value, the corresponding point should be the next point of the polygon. If there are multiple minimum values, the point with the highest x coordinate should be the next point. We can use this method to find the next points until we reach the point with the maximum x coordinate. At this time, we should search upwards or in the left direction. We stil want to minimze the slope value, but in case of multiple matches, we want the point with the lowest x coordinate. We continue in this manner until we reach our starting point again. Searching upwards and downwards is a special case, so instead of calculating slope values, we just identify the point with the lowest or highest y coordinate respectively. This is relevant only if there are mulpiple points on the left/right edge with the same x coordinate.

## Code Structure

### UI

The entry point of the program is the function OnInit in MyApp. MyApp is a wxApp and is responsible for creating the UI elements. It builds a new Frame, which in turn creates the three building blocks of the UI: the clear button, the draw polygon button and the canvas. Frame implements ButtonStateNotifier so that it can get notified of events that result in button status changes. The class Canvas is a wxPanel subclass, and is responsible for handling user input, displaying the pointset and the polygon if needed, and storing the model state. The model is represented by the class CanvasData, which stores the point set and the polygon, and notifies the UI of data changes. The UI works with the wxPoint data type, which defines the origin of the coordinate system in the "top-left corner", meaning the y coordinates are inverted.

### Logic

The Geometry.x files contain the necessary UI-independent logic and classes recquired to solve the task. A few functions are public and can be used outside the file. These functions are unit-tested. The remaining of the functions are helper funtions and are local to Geometry.cpp. All of this logic uses the conventional coordinate system where the origin is placed in the "bottom-left corner". The Logic.x files serve the purpose of communication between the UI and the Geometry functions. Here we can convert coordinates between the two coordinate systems, and we can execute additional checks.

## Possible Improvements

The function FindNextPointInBoundingPolygon expects SearchDirection as a parameter. This could be avoided by analyzing the point set further to identify the search direction locally in the function. This means we need to do additional calculations that are unnecessary in our use cases. We could solve the issue by providing both versions (one that expects the search direction from the caller, and another that does the calculations itself), but then we have another problem: what if the caller passes in the wrong information? I chose not to deal with this issue and have a function that expects the right search direction information, or otherwise does not guarantee the right result.

We could optimize the polygon computing algorithm in case we already have a polygon for an earlier point set, and only need to examine a few additional points. By saving the points used for the current polygon, we could check how the new points change the edges locally.

We can start the program in test mode by setting a bool flag but this is not the optimal solution. We should have a test program that can run separately from the main application without us having to touch the code.
