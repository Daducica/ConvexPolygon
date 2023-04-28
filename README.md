# Convex Bounding Polygon

## Task

The goal of this project is to implement an algorithm that computes the convex bounding polygon of a set of 2D points, and provide a user interface that allows the user to place points on a surface or delete them. Once the user is satisfied with the point set, they should be able to initiate the convex bounding polygon calculation, which would generate a polygonal shape that encloses all the points and has the smallest possible area. The resulting polygon should be drawn on the screen using a different color than the points. It should be possible to update the polygon, if the user wants to place additional points.

## Technicalities

I used Visual Studio (version Visual C++ 14.2) C++17 Standard and the wxWidgets library (version 3.2.2.1).

## Algorithm

The idea is to find the leftmost point in the point set, and then iterate through the points in a counter-clockwise manner by determining the next point based on the slope of the line connecting the current point to the next point.

We start by identifying the point that is located furthest to the left (or the bottom-most one, in case there are multiple points with the same minimum x value). Then we analyze the rest of the point set, and try to find the next point in the right direction. We connect each eligible point with the start point and determine the slope values for these lines. If there is only one minimum value, the corresponding point should be the next point of the polygon. If there are multiple minimum values, the point with the highest x coordinate should be the next point. We can use this method to find the next points until we reach the point with the maximum x coordinate. At this time, we should search upwards or in the left direction. We stil want to minimze the slope value, but in case of multiple matches, we want the point with the lowest x coordinate. We continue in this manner until we reach our starting point again. Searching upwards and downwards is a special case, so instead of calculating slope values, we just identify the point with the lowest or highest y coordinate respectively. This is relevant only if there are mulpiple points on the left/right edge with the same x coordinate.

## Code Structure


## Possible Improvements

