(define pi (acos -1))
(define n 15)

(define x0 0)
(define y0 0)

(define p1 0)


(define (polygon center p a n)
  (if (> n 0)
      (begin
	(set! p1 (Point "" rotation p center a))
	(send p1 masked)
	(Segment "" extremities p p1)
	(polygon center p1 a (- n 1)))))

(new-figure "Regular Polygon!")


(lets Point "C" free x0 y0)
(lets Numeric "a" free 0 0 (* 2 (/ pi n)))
(send a masked)

(set! p1 (Point "I" free 5 0))

(lets Segment "S" extremities C p1)
(Segment ""  rotation S C a)


(polygon C p1 a n)



