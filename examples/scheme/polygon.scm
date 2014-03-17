(define pi (acos -1))
(define x0 5)
(define y0 0)
(define n 10)
(define a (/ (* 2 pi) n))
(define p1 0)
(define p2 0)

(define (rotate x y a)
  (list (- (* x (cos a)) (* y (sin a)))
	(+ (* x (sin a)) (* y (cos a)))))

(define (polygon x y a n)
  (let ((rot (rotate x y a)))
    (if (> n 0)
	(begin 
	  (set! p2 (Point "" free (car rot) (cadr rot)))
	  (Segment "" extremities p1 p2)
	  (set! p1 p2)
	  (polygon (car rot) (cadr rot) a (- n 1))))))


(new-figure "Polygon")

(lets Point "O3" free 0 0)

(lets Point "I" free x0 y0)

(set! p1 I)

(polygon x0 y0 a n) 

