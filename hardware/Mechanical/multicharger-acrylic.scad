$fn = 200;

module baseboard (offset=[0,0,0]) {
    xoff = 1.4/2;
    yoff = 1.6/2;
    radius = (0.125-0.007)/2;
    color("blue") translate(offset) {
        translate([ xoff, yoff,0])circle(radius);
        translate([-xoff, yoff,0])circle(radius);
        translate([ xoff,-yoff,0])circle(radius);
        translate([-xoff,-yoff,0])circle(radius);
    }
}


// Design Plastic

difference() {
    // outline
color("blue") hull(){ 
    xoff = 4.25;
    yoff = (2.25)/2;
    radius = 0.2;
    translate([ xoff, yoff,0])circle(radius);
    translate([-xoff, yoff,0])circle(radius);
    translate([ xoff,-yoff,0])circle(radius);
    translate([-xoff,-yoff,0])circle(radius);
}
    // holes for baseboards

  for ( i = [-3 : 2 : 3]) {
      baseboard([i,0,0]);
  }

}