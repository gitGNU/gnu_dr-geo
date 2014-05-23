;;
;;  Dr Geo
;; (C) Copyright OFSET 2003
;; Author: Danilo Segan <dsegan@gmx.net>
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
;; Serbian version of constant; uses UTF-8, untested, provides both Latin
;; script and cyrillic script
;;

;; Basic definitions
(define-syntax дефиниши (syntax-rules () ((_ args ...) (define args ...))))
(define-syntax некасу (syntax-rules () ((_ args ...) (lets args ...))))
(define-syntax нека (syntax-rules () ((_ args ...) (let args ...))))
(define-syntax постави! (syntax-rules () ((_ args ...) (set! args ...))))
(define-syntax ако (syntax-rules () ((_ args ...) (if args ...))))
(define-syntax почни (syntax-rules () ((_ args ...) (begin args ...))))

(define-syntax definiši (syntax-rules () ((_ args ...) (define args ...))))
(define-syntax definisi (syntax-rules () ((_ args ...) (define args ...))))
(define-syntax nekasu (syntax-rules () ((_ args ...) (lets args ...))))
(define-syntax neka (syntax-rules () ((_ args ...) (let args ...))))
(define-syntax postavi! (syntax-rules () ((_ args ...) (set! args ...))))
(define-syntax ako (syntax-rules () ((_ args ...) (if args ...))))
(define-syntax pocni (syntax-rules () ((_ args ...) (begin args ...))))
(define-syntax počni (syntax-rules () ((_ args ...) (begin args ...))))


;; Color taken from geo/couleur.h
(define crna black)
(define tamno-siva dark-grey)
(define siva grey)
(define bela white)
(define tamno-zelena dark-green)
(define zelena green)
(define tamno-plava dark-blue)
(define plava blue)
(define crvena red)
(define žuta yellow)
(define zuta yellow)
  
(define црна black)
(define тамно-сива dark-grey)
(define сива grey)
(define бела white)
(define тамно-зелена dark-green)
(define зелена green)
(define тамно-плава dark-blue)
(define плава blue)
(define црвена red)
(define жута yellow)
  

;; Thickness taken from geo/drgeo_drgeoStyle.h
(define crtkano dashed)
(define tanko small)

(define црткано dashed)
(define танко small)


;; PointType taken from geo/drgeo_drgeoStyle.h
(define okrugla round)
(define krstić cross)
(define krstic cross)
(define prazna-okrugla round-empty)
(define prazan-kvadratić rec-empty)
(define prazan-kvadratic rec-empty)

(define округла round)
(define крстић cross)
(define празна-округла round-empty)
(define празан-квадратић rec-empty)

;; to avoid ' define some synonyms
;; related to style
(define boja 'color)
(define боја 'color)
(define дебљина 'thickness)
(define debljina 'thickness)
(define облик 'shape)
(define oblik 'shape)
(define величина 'size)
(define veličina 'size)
(define velicina 'size)
(define маскирано 'masked)
(define maskirano 'masked)
;; related to point
(define слободна 'free)
(define на-кривој 'on-curve)
(define средиште-2тачке 'middle-2pts)
(define средина-сегмента 'middle-segment)
(define slobodna 'free)
(define na-krivoj 'on-curve)
(define središte-2tačke 'middle-2pts)
(define srediste-2tacke 'middle-2pts)
(define sredina-segmenta 'middle-segment)
;; related to segment
(define екстремуми 'extremities)
(define ekstremumi 'extremities)
;;related to line, ray, circle, vector
(define паралелна 'parallel)
(define паралелан 'parallel)
(define paralelna 'parallel)
(define paralelan 'parallel)
(define upravna 'orthogonal)
(define upravan 'orthogonal)
(define upravno 'orthogonal)
(define upravni 'orthogonal)
;; related to numeric
(define дужина-сегмента 'segment-length)
(define норма-вектора 'vector-norm)
(define тачка-линије 'point-line)
(define dužina-segmenta 'segment-length)
(define duzina-segmenta 'segment-length)
(define norma-vektora 'vector-norm)
(define tačka-linije 'point-line)
(define tacka-linije 'point-line)
;; related to  angle
(define геометријски 'geometric)
(define geometrijski 'geometric)
;; related to transformation
(define увећање 'scale)
(define uvećanje 'scale)
(define uvecanje 'scale)
(define симетрија 'symmetry)
(define simetrija 'symmetry)


;;
;; Some Macro to get cooool way 
;; to define object:
;; 
;; (soit point "A" libre 1 2) is as
;; (define A (Point "A" free 1 2))
;;
(define-macro (тачка func . Largs)
`(define ,(string->symbol (car Largs)) (,func ,@Largs)))

(define-macro (tačka func . Largs)
`(define ,(string->symbol (car Largs)) (,func ,@Largs)))

(define-macro (tacka func . Largs)
`(define ,(string->symbol (car Largs)) (,func ,@Largs)))

;; 
;; Define synonyme in your languale file
;; (i.e. drgeo_scm_interface_constant_fr.scm)

(define нова-фигура new-figure)
(define нова-слика new-figure)
(define пошаљи send)
(define Права Line)
(define Зрак Ray)
(define Круг Circle)
(define Жижа Locus)
(define Вектор Vector)
(define Број Numeric)
(define Полигон Polygon)
(define Сегмент Segment)

(define nova-figura new-figure)
(define nova-slika new-figure)
(define pošalji send)
(define posalji send)
(define Prava Line)
(define Zrak Ray)
(define Krug Circle)
(define Žiža Locus)
(define Ziza Locus)
(define Vektor Vector)
(define Broj Numeric)
(define Poligon Polygon)
