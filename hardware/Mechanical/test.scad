$fn = 120;

filename = "test.dxf";

// offset is clearance around board
// negagive for inset, positive for exterior

module pcb(h,d,fname,c = false) {
translate([-11.135,-4.5,0])linear_extrude(height = h, center = c, convexity = 10)
       offset(r=d)import (file = fname)
   ;
}

// create expanded board + shrunk board for 
// plastic clearance

union(){
 pcb(2,0.2,filename);
 pcb(1,-0.2,filename,c=true);
}