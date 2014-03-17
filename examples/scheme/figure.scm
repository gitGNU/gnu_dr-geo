(define (triangle p1 p2 p3)
  (Segment "" extremities p1 p2)
  (Segment "" extremities p2 p3)
  (Segment "" extremities p1 p3))

(define (hasard)
  (- 8 (* 16 (random:uniform))))

(new-figure "Ma figure")

(lets Point "A" free (hasard) 0)
(lets Point "B" free 5 0)
(lets Point "C" free (hasard) 5)
(lets Point "D" free -5 (hasard))

(triangle A B C)
(soit Polygon "P1" npoints A B C D)
(envoi P1 couleur rouge)

(lets Point "I" middle-2pts A B)

(lets Line "l1" 2points A D)
(lets Ray "r" 2points D C)
(lets Vector "v1" 2points D I)
(lets Circle "Cc" 2points I B)
(Numeric "p" 'circle-length 5 0 Cc)
(lets Numeric "a1" free 1 2 3.1415)
(lets Point "M" rotation A B a1)
(lets Segment "S" extremities A B)
(lets Numeric "a2" vector-norm 1 1 v1)

(lets Point "In" intersection r S) 
(Line "(d1)" parallel In l1)
(Line "(d2)" orthogonal In l1)
(Point "I1" intersection r Cc)
(Point "I2" intersection2 r Cc)
;(Angle "alpha" geometric A B C)
(Segment "Scale" scale S D a1)
(lets Point "mobile" on-curve S .5)
(Arc "Arc" 3points C M B)

; Send message to change the attributes of our objects

(send Cc color red)
(send Cc thickness large)
(send A masked)
(send l1 thickness dashed)
(send B shape round)
(send B color bordeaux)
(send B size small)
