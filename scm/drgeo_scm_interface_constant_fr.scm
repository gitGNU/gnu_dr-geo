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
(define noir black)
(define gris-noir dark-grey)
(define gris grey)
(define blanc white)
(define vert-noir dark-green)
(define vert green)
(define bleu-noir dark-blue)
(define bleu blue)
(define rouge red)
(define jaune yellow)
  

;; Thickness taken from geo/drgeo_drgeoStyle.h
(define tiret dashed)
(define petit small)


;; PointType taken from geo/drgeo_drgeoStyle.h
(define rond round)
(define croix cross)
(define rond-vide round-empty)
(define rec-vide rec-empty)

;; to avoid ' define some synonyms
;; related to style
(define couleur 'color)
(define épaisseur 'thickness)
(define forme 'shape)
(define taille 'size)
(define masquer 'masked)
;; related to point
(define libre 'free)
(define sur-courbe 'on-curve)
(define sur-ligne 'on-curve)
(define milieu-2pts 'middle-2pts)
(define milieu-segment 'middle-segment)
;; related to segment
(define extrémités 'extremities)
;;related to line
(define parallèle 'parallel)
(define orthogonale 'orthogonal)
(define perpendiculaire 'orthogonal)
;; related to circle
(define centre-rayon 'center-radius)
(define centre-segment 'center-segment)
;; related to numeric
(define longueur-segment 'segment-length)
(define norme-vecteur 'vector-norm)
(define point-droite 'point-line)
(define point-cercle 'point-circle)
(define longueur-cercle 'circle-length)
(define pente-droite 'line-slope)
(define longueur-arc 'arc-length)
;; related to  angle
(define géométrique 'geometric)
(define orienté 'oriented)
;; related to transformation
(define homothétie 'scale)
(define symétrie 'symmetry)


;;
;; Some Macro to get cooool way 
;; to define object:
;; 
;; (soit point "A" libre 1 2) is as
;; (define A (Point "A" free 1 2))
;;
(define-macro (soit func . Largs)
`(define ,(string->symbol (car Largs)) (,func ,@Largs)))


;; 
;; Define synonyme in your languale file
;; (i.e. drgeo_scm_interface_constant_fr.scm)

(define nouvelle-figure new-figure)
(define envoi send)
(define Droite Line)
(define Demi-droite Ray)
(define Cercle Circle)
(define Arc-cercle Arc)
(define Lieu Locus)
(define Vecteur Vector)
(define Nombre Numeric)
(define Polygone Polygon)