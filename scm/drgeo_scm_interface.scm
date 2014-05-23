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
;; Some global variable
;;

(define currentFigure 0)


;;
;;
;;
(define (drgeo-error . L)
  (display "** Error when parsing the figure:")
  (newline)
  (for-each display L)
  (newline)
  (exit))


;;
;; return the id list
;;
(define (idList Lopt)
  (if (null? Lopt)
      '()
      (cons (send (car Lopt) 'id) (idList (cdr Lopt)))))

;;
;; Figure construcutor, it's mapped to DrGeo
;;
(define (new-figure name)
  (set! currentFigure (drgeoMake-figure name)))

;;
;; Helper to the prototype
;;
(define (send prototype message . Largs)
  (apply prototype (cons message Largs)))

(define (delegate message prototype Largs)
  (apply prototype (cons message Largs)))


;;
;; The base prototype to all geometric object
;;
(define (Geometric-Object myName type . Lopt)
  ;; id is the address of the object
  (let ((id 0)
        (name ""))
    (letrec ((init
	      (lambda ()
		;; instantiate the base object
		(case type
		  ;; object, center, angle
		  ((rotation) (set! id (drgeoMake-rotation
					currentFigure myName
					(send (caar Lopt) 'id)   
					(send (cadar Lopt) 'id)
					(send (caddar Lopt) 'id))))
		  ;; object, center, factor
		  ((scale) (set! id (drgeoMake-scale 
				     currentFigure myName
				     (send (caar Lopt) 'id)   
				     (send (cadar Lopt) 'id)
				     (send (caddar Lopt) 'id))))		  
		  ;; object, center 
		  ((symmetry) (set! id (drgeoMake-symmetry
					currentFigure myName
					(send (caar Lopt) 'id)   
					(send (cadar Lopt) 'id))))
		  ;; object, axe
		  ((reflexion) (set! id (drgeoMake-reflexion
					currentFigure myName
					(send (caar Lopt) 'id)   
					(send (cadar Lopt) 'id))))
		  ;; object, vector
		  ((translation) (set! id (drgeoMake-translation
					   currentFigure myName
					   (send (caar Lopt) 'id)   
					   (send (cadar Lopt) 'id)))))
		(set! name myName)))
	     (self
	      (lambda (message . Largs)
                (case message
                  ((init) (set! id (car Largs)))
		  ((id) id)
                  ((name) name)
                  ((set-name!) (set! name (car Largs)))
                  ((color) (change-color currentFigure id (car Largs)))
                  ((thickness) (change-thickness currentFigure id (car Largs)))
                  ((size) (change-thickness currentFigure id (car Largs)))
                  ((shape) (change-pointShape currentFigure id (car Largs)))
                  ((masked) (change-masked currentFigure id))
                  ((move) (display "move"))))))
      (init)
      self)))
                      
;;
;; The Point prototype
;;
(define (Point myName type . Lopt)
  (let ((parent (Geometric-Object myName type Lopt)))
    (letrec ((init 
              (lambda ()
                ;; from here instantiate a point object
                (case type
                  ((free) (send parent 'init (drgeoMake-freePoint currentFigure myName 
							     (car Lopt) (cadr Lopt))))
                  ((on-curve) (send parent 'init (drgeoMake-pointOnCurve 
						  currentFigure myName
						  (send (car Lopt) 'id) (cadr Lopt))))
                  ((middle-2pts) (send parent 'init (drgeoMake-middle2Points 
						     currentFigure myName
						     (send (car Lopt) 'id)
						     (send (cadr Lopt) 'id))))
                  ((middle-segment) (send parent 'init (drgeoMake-middleSegment 
							currentFigure myName
							(send (car Lopt) 'id))))
		  ((coordinate) '())
		  ;; line, line (no circle and arc)
		  ((intersection) (send parent 'init (drgeoMake-intersection
						     currentFigure myName
						     (send (car Lopt) 'id)
						     (send (cadr Lopt) 'id) 0)))
		  ;; object, object, (n = 0 or 1) for the intersection point number
		  ((intersection2) (send parent 'init (drgeoMake-intersection
						       currentFigure myName
						       (send (car Lopt) 'id)
						       (send (cadr Lopt) 'id) 1))))))
	     (self
	      (lambda (message . Largs)
                (case message
		  (else (delegate message parent Largs))))))
      (init)
      self)))

;;
;; The Segment prototype
;;
(define (Segment myName type . Lopt)
  (let ((parent (Geometric-Object myName type Lopt)))
    (letrec ((init 
              (lambda ()
                ;; from here instantiate a segment object
                (case type
                  ((extremities) (send parent 'init (drgeoMake-segmentExtremity 
						   currentFigure myName 
						   (send (car Lopt) 'id)
						   (send (cadr Lopt)'id)))))))
             (self
              (lambda (message . Largs)
                (case message
		  (else (delegate message parent Largs))))))
      (init)
      self)))

;;
;; The straight line prototype
;;
(define (Line myName type . Lopt)
  (let ((parent (Geometric-Object myName type Lopt)))
    (letrec ((init 
              (lambda ()
                ;; from here instantiate Line object
                (case type
                  ((2points) (send parent 'init (drgeoMake-line currentFigure myName 
							    (send (car Lopt) 'id)
							    (send (cadr Lopt)'id))))
		  ; point, direction
		  ((parallel) (send parent 'init (drgeoMake-parallel currentFigure myName 
								(send (car Lopt) 'id)
								(send (cadr Lopt) 'id))))
		  ; point, direction
		  ((orthogonal) (send parent 'init (drgeoMake-orthogonal currentFigure myName 
								    (send (car Lopt) 'id)
								    (send (cadr Lopt) 'id)))))))
	     (self
              (lambda (message . Largs)
                (case message
		  (else (delegate message parent Largs))))))
      (init)
      self)))
;;
;; The Ray prototype
;;
(define (Ray myName type . Lopt)
  (let ((parent (Geometric-Object myName type Lopt)))
    (letrec ((init 
              (lambda ()
                ;; from here instantiate Ray object
                (case type
                  ((2points) (send parent 'init (drgeoMake-ray currentFigure myName 
							  (send (car Lopt) 'id)
							  (send (cadr Lopt)'id)))))))
             (self
              (lambda (message . Largs)
                (case message
		  (else (delegate message parent Largs))))))
      (init)
      self)))


;;
;; The vector prototype
;;
(define (Vector myName type . Lopt)
  (let ((parent (Geometric-Object myName type Lopt)))
    (letrec ((init 
              (lambda ()
                ;; from here instantiate Vector object
                (case type
                  ((2points) (send parent 'init (drgeoMake-vector currentFigure myName 
							     (send (car Lopt) 'id)
							     (send (cadr Lopt) 'id)))))))
             (self
              (lambda (message . Largs)
                (case message
		  (else (delegate message parent Largs))))))
      (init)
      self)))

;;
;; The circle prototype
;;
(define (Circle myName type . Lopt)
  (let ((parent (Geometric-Object myName type Lopt)))
    (letrec ((init 
              (lambda ()
                ;; from here instantiate a Circle object
                (case type
		  ;; center , point
                  ((2points) (send parent 'init (drgeoMake-circle2Points currentFigure myName 
								    (send (car Lopt) 'id)
								    (send (cadr Lopt)'id))))
		  ;; center, numeric
		  ((center-radius) (send parent 'init (drgeoMake-circleRadius currentFigure myName 
									 (send (car Lopt) 'id)
									 (send (cadr Lopt)'id))))
		  ;; center, segment
		  ((center-segment) (send parent 'init (drgeoMake-circleSegment currentFigure myName 
									   (send (car Lopt) 'id)
									   (send (cadr Lopt)'id)))))))
             (self
              (lambda (message . Largs)
                (case message
		  (else (delegate message parent Largs))))))
      (init)
      self)))

;;
;; The circle prototype
;;
(define (Arc myName type . Lopt)
  (let ((parent (Geometric-Object myName type Lopt)))
    (letrec ((init 
              (lambda ()
                ;; from here instantiate a Circle object
                (case type
		  ;; point , point, point
                  ((3points) (send parent 'init (drgeoMake-arc currentFigure myName 
							  (send (car Lopt) 'id)
							  (send (cadr Lopt) 'id)
							  (send (caddr Lopt) 'id)))))))
	     (self
	      (lambda (message . Largs)
		(case message
		  (else (delegate message parent Largs))))))
      (init)
      self)))
;;
;; The Locus prototype
;;
(define (Locus myName type . Lopt)
  (let ((parent (Geometric-Object myName type Lopt)))
    (letrec ((init 
              (lambda ()
                ;; from here instantiate a Locus object
		;; mobile point on a curve, constrained point
                (case type
                  ((2points) (send parent 'init (drgeoMake-locus currentFigure myName 
							    (send (car Lopt) 'id)
							    (send (cadr Lopt) 'id)))))))
	     (self
	      (lambda (message . Largs)
                (case message
		  (else (delegate message parent Largs))))))
      (init)
      self)))
;;
;; The Polygon prototype
;;
(define (Polygon myName type . Lopt)
  (let ((parent (Geometric-Object myName type Lopt)))
    (letrec ((init 
              (lambda ()
                ;; from here instantiate a Polygon object
                (case type
                  ((npoints) (send parent 'init (drgeoMake-polygon currentFigure myName 
							      (idList Lopt)))))))
	     (self
	      (lambda (message . Largs)
                (case message
		  (else (delegate message parent Largs))))))
      (init)
      self)))
;;
;; The numeric line prototype
;;
(define (Numeric myName type . Lopt)
  (let ((parent (Geometric-Object myName type Lopt)))
    (letrec ((init 
              (lambda ()
                ;; from here instantiate numeric object
                (case type
		  ;; x, y, value
                  ((free) (send parent 'init (drgeoMake-freeValue currentFigure myName 
							     (car Lopt) (cadr Lopt)
							     (caddr Lopt))))
		  ;; x, y, segment
                  ((segment-length) (send parent 'init (drgeoMake-segmentLength currentFigure myName 
									   (car Lopt) (cadr Lopt)
									   (send (caddr Lopt) 'id))))
		  ;; x ,y, vector
                  ((vector-norm) (send parent 'init (drgeoMake-vectorNorm currentFigure myName 
								     (car Lopt) (cadr Lopt)
								     (send (caddr Lopt) 'id))))
		  ;; x, y, point, circle
                  ((point-circle) (send parent 'init (drgeoMake-distancePointCircle 
						      currentFigure myName 
						      (car Lopt) (cadr Lopt)
						      (send (caddr Lopt) 'id) 
						      (send (cadddr Lopt) 'id))))
		  ;; x, y, point, line
		  ((point-line) (send parent 'init (drgeoMake-distancePointLine
						    currentFigure myName 
						    (car Lopt) (cadr Lopt)
						    (send (caddr Lopt) 'id) 
						    (send (cadddr Lopt) 'id))))
		  ;; x, y, point, point
		  ((point-point) (send parent 'init (drgeoMake-distancePointPoint
						     currentFigure myName 
						     (car Lopt) (cadr Lopt)
						     (send (caddr Lopt) 'id) 
						     (send (cadddr Lopt) 'id))))
		  ;; x, y, circle
		  ((circle-length) (send parent 'init (drgeoMake-circleLength
						       currentFigure myName 
						       (car Lopt) (cadr Lopt)
						       (send (caddr Lopt) 'id))))
		  ;; x, y, line
		  ((line-slope) (send parent 'init (drgeoMake-lineSlope
						    currentFigure myName 
						    (car Lopt) (cadr Lopt)
						    (send (caddr Lopt) 'id))))
		  ;; x, y, arc
		  ((arc-length) (send parent 'init (drgeoMake-arcLength
						    currentFigure myName 
						    (car Lopt) (cadr Lopt)
						    (send (caddr Lopt) 'id)))))))
	     (self
              (lambda (message . Largs)
                (case message
		  (else (delegate message parent Largs))))))
      (init)
      self)))
;;
;; The angle line prototype
;;
(define (Angle myName type . Lopt)
  (let ((parent (Geometric-Object myName type Lopt)))
    (letrec ((init 
              (lambda ()
                ;; from here instantiate angle object
                (case type
		  ;; p1, p2, p3
                  ((geometric) (send parent 'init (drgeoMake-geometricAngle currentFigure myName 
								       (send (car Lopt) 'id)
								       (send (cadr Lopt) 'id)
								       (send (caddr Lopt) 'id))))
		  ;; x, y, v1, v2
		  ((oriented) (send parent 'init (drgeoMake-orientedAngle currentFigure myName 
								     (car Lopt) (cadr Lopt)
								     (send (caddr Lopt) 'id)
								     (send (cadddr Lopt) 'id)))))))
	     (self
              (lambda (message . Largs)
                (case message
		  (else (delegate message parent Largs))))))
      (init)
      self)))

;;
;; Our test suite
;;

;(define (drgeoMake-figure name)
;  (set! currentFigure name)
;  (display currentFigure)
;  (newline)
;  currentFigure)

;(define (drgeoMake-intersection fig name o1 o2 k)
;  (display "rotation: ")
;  (display fig)
;  (display name)
;  (display o1)
;  (display o2)
;  (display k)
;  5454)

;(define (drgeoMake-freeValue fig name x y v)
;  (display fig)
;  (display name)
;  (display x)
;  (display y)
;  (display v)
;  5454)


;(new-figure "Ma figure")
;(define a (Numeric "a" 'free 0 0 1))
;(define p (Point "o" 'intersection a a))
