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
;; Spanish version out of the french one  by Adrian Soto 2004. 
;; (adrianmatematico@yahoo.com.mx)

;; I think it might be nice to have a version of the commands having no 
;; accents nor e\~nes, so I included it.  Some adjectives have both masculin
;; and femenin versions.  I am not sure if it is a good idea.



;; Color taken from geo/couleur.h
(define negro black)
(define gris-oscuro dark-grey)
(define gris-obscuro dark-grey)
(define gris grey)
(define blanco white)
(define verde-oscuro dark-green)
(define verde-obscuro dark-green)
(define verde green)
(define azul-oscuro dark-blue)
(define azul-obscuro dark-blue)
(define azul blue)
(define rojo red)
(define rojo-oscuro bordeaux)
(define rojo-obscuro bordeaux)
(define amarillo yellow)
(define anaranjado orange)
(define naranja orange)

  

;; Thickness taken from geo/drgeo_drgeoStyle.h
(define punteado dashed)
(define pequeño small)
(define pequeno small)
(define grande large)
(define normal normal)
;; "normal" is the same in english, french and spanish.




;; PointType taken from geo/drgeo_drgeoStyle.h
(define redondo round)
(define redonda round)
(define cruz cross)
(define cuadrado rec)
(define cuadrada rec)
(define redondo-vacío round-empty)
(define redondo-vacio round-empty)
(define redonda-vacía round-empty)
(define redonda-vacia round-empty)
(define cuadrado-vacío rec-empty)
(define cuadrado-vacio rec-empty)
(define cuadrada-vacía rec-empty)
(define cuadrada-vacia rec-empty)
(define rec-vacía rec-empty)
(define rec-vacia rec-empty)


;; to avoid ' define some synonyms
;; related to style
(define color 'color)
(define grosor 'thickness)
(define forma 'shape)
(define tamaño 'size)
(define tamano 'size)
(define enmascarado 'masked)
(define oculto 'masked)
(define ocultar 'masked)

;; related to point
(define libre 'free)
(define sobre-la-curva 'on-curve)
(define sobre-curva 'on-curve)
(define en-curva 'on-curve)
(define en-linea 'on-curve)
(define mitad-2pts 'middle-2pts)
(define mitad-segmento 'middle-segment)
(define intersección 'intersection)
(define interseccion 'intersection)
(define intersección2 'intersection2)
(define interseccion2 'intersection2)

;; related to segment
(define extremos 'extremities)

;;related to line, ray, circle, vector
(define 2puntos '2points)
(define 2pts '2points)
(define 3points '3points)
(define 3pts '3points)
;;related to line
(define paralela 'parallel)
(define ortogonal 'orthogonal)
(define perpendicular 'orthogonal)
;; related to circle
(define centro-radio 'center-radius)
(define centro-segmento 'center-segment)
;; related to poligon
(define npuntos 'npoints)
;; related to numeric
(define longitud-segmento 'segment-length)
(define norma-vector 'vector-norm)
(define longitud-vector 'vector-norm)
(define punto-recta 'point-line)
(define punto-círculo 'point-circle)
(define punto-circulo 'point-circle)
(define punto-punto 'point-point)
(define perímetro-círculo 'circle-length)
(define perimetro-circulo 'circle-length)
(define longitud-círculo 'circle-length)
(define longitud-circulo 'circle-length)
(define pendiente-línea 'line-slope)
(define pendiente-linea 'line-slope)
(define pendiente-recta 'line-slope)
(define longitud-arco 'arc-length)

;; related to  angle
(define geométrico 'geometric)
(define geometrico 'geometric)
(define orientado 'oriented)

;; related to transformation
(define rotación 'rotation)
(define rotacion 'rotation)
(define homotecia 'scale)
(define escala 'scale)
(define simetría 'symmetry)
(define simetria 'symmetry)
;;(define media-vuelta 'symmetry)
(define reflexion 'reflexion)
(define translación 'translation)
(define translación 'translation)
(define traslación 'translation)
(define traslacion 'translation)

;;(define media-vuelta 'symmetry)




;;
;; Some Macro to get cooool way 
;; to define object:
;; 
;; (sea Punto "A" libre 1 2) is as
;; (define A (Punto "A" libre 1 2))
;;
(define-macro (sea func . Largs)
`(define ,(string->symbol (car Largs)) (,func ,@Largs)))


;; 
;; Define synonyme in your languale file
;; (i.e. drgeo_scm_interface_constant_fr.scm)

(define nueva-figura new-figure)
(define figura-nueva new-figure)
(define enviar send)
(define envía send)
(define envia send)
(define Punto Point)
(define Recta Line)
(define Línea Line)
(define Linea Line)
(define Segmento Segment)
(define Semirrecta Ray)
(define Rayo Ray)
(define Círculo Circle)
(define Circulo Circle)
(define Arco Arc)
(define Arco Arc)
(define Lugar-geométrico Locus)
(define Lugar-geometrico Locus)
(define Vector Vector)
(define Numérico Numeric)
(define Numerico Numeric)
(define Número Numeric)
(define Numero Numeric)
(define Ángulo Angle)
(define Angulo Angle)
(define Polígono Polygon)
(define Poligono Polygon)