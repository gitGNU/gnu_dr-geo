;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Dr. Geo plug-in for TeXmacs
;;
;; (c) Copyright Hilaire Fernandes 2003
;; (c) Copyright Andrian Soto Ulises 2004,2005
;;
;; This software falls under the GNU general public license and comes WITHOUT
;; ANY WARRANTY WHATSOEVER. 
;; If you don't have this file, write to the Free Software Foundation, Inc.,
;; 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;(define (secure-initialize)
;;   (import-from (secure-secure)))
;;(define show-instructions
;;   (lambda ()
;;     (insert "Function not available at the moment")))
;;(define show-version
;;   (lambda ()
;;     (insert "Function not available at the moment")))

(menu-extend texmacs-extra-menu
  (if (equal? (get-env "prog-language") "drgeo")
      (=> "DrGeo" (link drgeo-menu))))

(menu-bind drgeo-menu
;  ("Version" (insert "Function not available now"))
;  ("Instructions" (insert "Fucntion not available now"))
  ("Newfigure" (insert " (new-figure \"<figuretitle>\")"))
  ---
  ("Lets (caption=name)" (insert "(lets <object> \"<name>\"  <etc>)"))
  ("Define(caption could be different)" (insert "(define <name> (<object> \"<caption>\" <etc>))"))
  ---
  (-> "Points" 
     ("Given by their coordinates" 
        (insert " (Point \"<caption>\" 'free <xcoordinate> <ycoordinate>)"))
     ("Point on a curve"
        (insert " (Point \"<caption>\" 'on-curve <curve> <number>)"))
     ("Midpoint of two points" 
        (insert " (Point \"<caption>\" 'middle-2pts <Point1> <Point2>)"))
     ("Midpoint of a segment" 
        (insert " (Point \"<caption>\" 'middle-segment <Segment>)"))
     ("Intersection of two curves"
        (insert " (Point \"<caption>\" 'intersection <curve1> <curve2>)"))
     ("Second intersection of two curves"
        (insert " (Point \"<caption>\" 'intersection2 <curve1> <curve2>)")))
  (-> "Straight Lines" 
     ("Passing through two points" 
        (insert " (Line \"<caption>\" '2points <point1> <point2>)"))
     ("Passing trough a point and parallel to a segment, vector, ray or line" 
        (insert " (Line \"<caption>\" 'parallel <point> <direction>)"))
     ("Passing trough a point and perpendicular to a segment, vector, ray or line" 
        (insert " (Line \"<caption>\" 'perpendicular <point> <direction>)")))
  (-> "Angles"
     ("Given three points"
       (insert " (Angle \"<caption>\" 'geometric <point1> <point2> <point3>)")))
  (-> "Rays" 
     ("From one point towards another" 
        (insert " (Ray \"<caption>\" '2points <point1> <point2>)")))
  (-> "Segments"
     ("Defined by its extremities"
        (insert " (Segment \"<caption>\" 'extremities <point1> <point2>)")))
  (-> "Vectors"
     ("Defined by two points"
        (insert " (Vector \"<caption>\" '2points <point1> <point2>)")))
  (-> "Numeric Objects"
     ("Given their position in the figure and its value"
        (insert " (Numeric \"<caption>\" 'free <x> <y> <number>)"))
     ("Given their position in the figure and a SEGMENT (returns LENGTH)"
        (insert " (Numeric \"<caption>\" 'segment-length <x> <y> <segment>)"))
     ("Given their position in the figure and a VECTOR (returns NORM of the vector)"
        (insert " (Numeric \"<caption>\" 'vector-norm <x> <y> <vector>)"))
     ("Given their position in the figure, a POINT and a LINE (returns DISTANCE)"
        (insert " (Numeric \"<caption>\" 'point-line <x> <y> <point> <line>)"))
     ("Given their position in the figure a POINT and a CIRCLE (returns DISTANCE)"
        (insert " (Numeric \"<caption>\" 'point-circle <x> <y> <point> <circle>)"))
     ("Given their position in the figure a POINT and a POINT (returns DISTANCE)"
        (insert " (Numeric \"<caption>\" 'point-point <x> <y> <point1> <point2>)"))
     ("Given their position in the figure a CIRCLE (returns PERIMETER)"
        (insert " (Numeric \"<caption>\" 'circle-length <x> <y> <circle>)"))
     ("Given their position in the figure a LINE (returns SLOPE)"
        (insert " (Numeric \"<caption>\" 'line-slope <x> <y> <line>)"))
     ("Given their position in the figure a ARC (returns LENGTH)"
        (insert " (Numeric \"<caption>\" 'arc-length <x> <y> <arc>)")))
  (-> "Circles"
     ("With a given center and passing through a point"
        (insert " (Circle \"<caption>\" '2points <point1> <point2>)"))
     ("With a given center and a radius given by a Numeric object"
        (insert " (Circle \"<caption>\" 'center-radius <point> <numeric>)"))
     ("With a given center and a radius given by a Segment"
        (insert " (Circle \"<caption>\" 'center-segment <point> <segment>)")))
  (-> "Arcs of a circle"
     ("Passing throuh three given points"
        (insert " (Arc \"<caption>\" '3points <point1> <point2> <point3>)")))
  (-> "Locus of points"
     ("Given by an independent point and a dependent point"
        (insert " (Locus \"<caption>\" '2points <point1> <point2>)")))
  (-> "Polygon"
     ("Passing through n-points"
        (insert " (Polygon \"<caption>\" 'npoints <point1> <point2> <etc>)")))
  (-> "Geometric Transformations"
     ("Translation by a vector"
        (insert " (<new-object> \"<caption>\" 'translation <oldobject> <vector>)"))
     ("Rotation given a center and an angle"
        (insert " (<new-object> \"<caption>\" 'rotation <oldobject> <Point> <angle>)"))
     ("Dilatation(Scale, Homothecy) given a center and a Numeric object (factor)"
        (insert " (<new-object> \"<caption>\" 'scale <oldobject> <point> <numericobject>)"))
     ("Symmetry (Half-turn) given a center"
        (insert " (<new-object> \"<caption>\" 'symmetry <oldobject> <point>)"))
     ("reflection on a line"
        (insert " (<new-object> \"<caption>\" 'reflection <oldobject> <line>)")))
  (-> "Change styles of objects"
     ("Changing color"
        (insert " (send <object> 'color <color>)"))
     ("Changing thickness of curves"
        (insert " (send <object> 'thickness <thickness>)"))
     ("Changing point shape and size"
        (insert " (send <object> 'shape <shape>)"))
     (-> "Changing visibility"
        ("Make it invisible" 
           (insert " (send <object> 'masked)"))
        ("Make it visible"
           (insert " (send <object> 'unmasked)")))
     (-> "Available colors"
         ("black=0" (insert "black"))
         ("dark-grey=1" (insert "dark-grey"))
         ("grey=2" (insert "grey"))
         ("white=3" (insert "white"))
         ("dark-green=4" (insert "dark-green"))
         ("green=5" (insert "green"))
         ("dark-blue=6" (insert "dark-blue"))
         ("blue=7" (insert "blue"))
         ("bordeaux=8" (insert "bordeaux"))
         ("red=9" (insert "red"))
         ("orange=10" (insert "orange"))
         ("yellow=11" (insert "yellow")))
     (-> "Available sizes and thickness"
         ("dashed=0" (insert "dashed"))
         ("small=0"  (insert "small"))
         ("normal=1" (insert "normal"))
	 ("large=2"  (insert "large")))
     (-> "Available shapes"
         ("round=0" (insert "round"))
         ("cross=1" (insert "cross"))
         ("rectangle=2" (insert "rec"))
         ("round-empty=3" (insert "round-empty"))
         ("rectangle-empty=4" (insert "rec-empty")))))


(plugin-configure drgeo
  (:require (url-exists-in-path? "drgeo"))
  (:launch "tm_drgeo --texmacs")  
  (:session "Dr. Geo"))