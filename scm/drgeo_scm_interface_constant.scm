;;
;;  Dr Geo
;; (C) Copyright OFSET 2003
;; Author: hilaire@ofset.org 
;; 
;;
;;
;;
;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public Licences as by published
;; by the Free Software Foundation; either version 2; or (at your option)
;; any later version
;;
;; This program is distributed in the hope that it will entertaining,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of 
;; MERCHANTABILTY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
;; Publis License for more details.
;;
;; You should have received a copy of the GNU General Public License along
;; with this program; if not, write to the Free Software Foundation, Inc.
;; 675 Mass Ave, Cambridge, MA 02139, USA.
;;

;; Color taken from geo/couleur.h
(define black 0)
(define dark-grey 1)
(define grey 2)
(define white 3)
(define dark-green 4)
(define green 5)
(define dark-blue 6)
(define blue 7)
(define bordeaux 8)
(define red 9)
(define orange 10)
(define yellow 11)
  

;; Thickness taken from geo/drgeo_drgeoStyle.h
(define dashed 0)
(define small 0)
(define normal 1)
(define large 2)

;; PointType taken from geo/drgeo_drgeoStyle.h
(define round 0)
(define cross 1)
(define rec 2)
(define round-empty 3)
(define rec-empty 4)

;; to avoid ' define some synonyms
;; related to style
(define color 'color)
(define thickness 'thickness)
(define shape 'shape)
(define size 'size)
(define masked 'masked)
;; related to point
(define free 'free)
(define on-curve 'on-curve)
(define on-line 'on-curve)
(define middle-2pts 'middle-2pts)
(define middle-segment 'middle-segment)
(define intersection 'intersection)
(define intersection2 'intersection2)
;; related to segment
(define extremities 'extremities)
;;related to line, ray, circle, vector
(define 2points '2points)
(define 2pts '2points)
(define 3points '3points)
(define 3pts '3points)
;;related to line
(define parallel 'parallel)
(define orthogonal 'orthogonal)
(define perpendicular 'orthogonal)
;; related to circle
(define center-radius 'center-radius)
(define center-segment 'center-segment)
;; related to polygon
(define npoints 'npoints)
;; related to numeric
(define segment-length 'segment-length)
(define vector-norm 'vector-norm)
(define point-line 'point-line)
(define point-circle 'point-circle)
(define point-point 'point-point)
(define circle-length 'circle-length)
(define line-slope 'line-slope)
(define arc-length 'arc-length)
;; related to angle
(define geometric 'geometric)
(define oriented 'oriented)
;; related to transformation
(define rotation 'rotation)
(define scale 'scale)
(define symmetry 'symmetry)
(define reflexion 'reflexion)
(define translation 'translation)

;;
;; Some Macro to get cooool way 
;; to define object:
;; 
;; (lets point "A" free 1 2) is as
;; (define A (Point "A" free 1 2))
;;
(define-macro (lets func . Largs)
`(define ,(string->symbol (car Largs)) (,func ,@Largs)))


;; 
;; Define synonym in your languale file
;; (i.e. drgeo_scm_interface_constant_fr.scm)

;; (define xxx new-figure)
;; (define xxx send)
;; (define xxx Point)
;; (define xxx Line)
;; (define xxx Segment)
;; (define xxx Ray)
;; (define xxx Circle)
;; (define xxx Arc)
;; (define xxxx Locus)
;; (define xxx Polygon)
;; (define xxx Vector)
;; (define xxx Numeric)
;; (define xxxx Angle)
