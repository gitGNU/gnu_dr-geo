(new-figure "Baum")

(lets Numeric "A1" free 2 2 +3.4)
(lets Numeric "A2" free 2 3 -3.7)
(lets Numeric "S1" free 2 4 +0.5)
(lets Numeric "S2" free 2 5 +0.9)

(define (dec n)
  (- n 1))

(define (inc n)
  (+ n 1))

(define (invisible p)
  (send p masked)
  p)

(define (scalerot oP C a s)
  (let* ((sP (invisible (Point "" scale oP C s)))
         (rP (invisible (Point "" rotation sP C a))) )
    rP))

(define (Zweig p0 p1 n)
  (Segment "" extremities p0 p1)
  (let* ((left-scale (if (odd? n) S1 S2))
         (left-angle A1)
         (right-scale (if (odd? n) S2 S1))
         (right-angle A2) )
    (if (> n 0)
      (begin
        (Zweig p1 (scalerot p0 p1  left-angle  left-scale) (dec n))
        (Zweig p1 (scalerot p0 p1 right-angle right-scale) (dec n))))))
	
(lets Point "A" free -3 -2)
(lets Point "B" free -3 0)

(Zweig A B 6)



