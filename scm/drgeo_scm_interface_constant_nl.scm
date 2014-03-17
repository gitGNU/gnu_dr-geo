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

;;
;; French version of the constant
;;

;; Color taken from geo/couleur.h
(define zwart black)
(define donkergrijs dark-grey)
(define grijs grey)
(define wit white)
(define donkergroen dark-green)
(define groen green)
(define donkerblauw dark-blue)
(define blauw blue)
(define rood red)
(define geel yellow)
  

;; Thickness taken from geo/drgeo_drgeoStyle.h
(define streepje dashed)
(define dun small)


;; PointType taken from geo/drgeo_drgeoStyle.h
(define rond round)
(define kruis cross)
(define holrond round-empty)
(define rec-leeg rec-empty) ;; FIXME ? wat wordt bedoeld ?

;; to avoid ' define some synonyms
;; related to style
(define kleur 'color)
(define dikte 'thickness)
(define vorm 'shape)
(define grootte 'size)
(define verbergen 'masked) ;; verborgen ??
;; related to point
(define willekeurig 'free)
(define op-kromme 'on-curve)
(define midden-2ptn 'middle-2pts)
(define midden-lijnstuk 'middle-segment)
;; related to segment
(define grenspunten 'extremities)
;;related to line
(define evenwijdig 'parallel)
(define orthogonaal 'orthogonal) ;; was orthog.
(define loodrecht 'orthogonal) ;; was perpend.
;; related to circle
(define middelpunt-straal 'center-radius)
(define middelpunt-lijnstuk 'center-segment)
;; related to numeric
(define lengte-lijnstuk 'segment-length)
(define norm-vector 'vector-norm)
(define punt-rechte 'point-line)
(define punt-cirkel 'point-circle)
(define omtrek-cirkel 'circle-length) 
(define rico-rechte 'line-slope)
(define lengte-boog 'arc-length)
;; related to  angle
(define meetkundig 'geometric)
(define georiënteerd 'oriented)
;; related to transformation
(define homothetie 'scale)
(define symmetrie 'symmetry)


;;
;; Some Macro to get cooool way 
;; to define object:
;; 
;; (soit point "A" libre 1 2) is as
;; (define A (Point "A" free 1 2))
;;
(define-macro (weze func . Largs)
`(define ,(string->symbol (car Largs)) (,func ,@Largs)))


;; 
;; Define synonyme in your languale file
;; (i.e. drgeo_scm_interface_constant_fr.scm)

(define nieuwe-figuur new-figure)
(define zend send)
(define Rechte Line)
(define Halfrechte Ray)
(define Cirkel Circle)
(define Cirkelboog Arc)
(define Plaats Locus)
(define Vector Vector)
(define Getal Numeric)
(define Veelhoek Polygon)