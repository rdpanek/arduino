pruchodkaProDiodu();
//usbProstup();
//horniKrabicka();
//spodniOblaKrabicka();

module horniKrabicka() {
    rotate([0,180,0]) {
        translate([0,0,-50.5]) horniOblaKrabicka();
    }
}

module spodniOblaKrabicka() {
    roundedRect([76, 56, 1.5], 5, $fn=40);
    difference() {
        translate([0,0,1.5]) roundedRect([74.5, 54.5, 2], 5, $fn=40);
        translate([0,0,1.5]) roundedRect([70, 50, 2], 5, $fn=40);
    }
    translate([-38,-28,0]) sloupky();
}


module horniOblaKrabicka() {
    difference() {
        difference(){
            roundedRect([76, 56, 27], 5, $fn=40);
            translate([0,0,1]) roundedRect([72, 52, 27], 5, $fn=40);
            translate([0,0,25]) roundedRect([74.5, 54.5, 2], 5, $fn=40);
        }
        translate([-11.5,2,0]) pruchodkaProDiodu();
            
        rotate([0,180,0]) {
            translate([-37,-26.8,-28])usbProstup();
        }
    }
    %translate([-11.5,2,0]) pruchodkaProDiodu();
    translate([-11.5,2,2]) ObjimkapruchodkaProDiodu();
    //translate([0,0,-0.2])openHardware();
    translate([-109,-26.8,7]) usbVnitrniRantl();
}


module openHardware(){

    import("/home/rdpanek/HTDOCS/gcodes/customs/Open_Hardware_Logo_for_objects_OpenSCAD/oshw.stl", convexity=10);
}

module pruchodkaProDiodu() {
    translate([0,0,0.5]) sphere(d=6.5,$fn=30);
    difference() {
        translate([0,0,3]) cylinder(h=4,d=6.5,$fn=30,center=true);
        translate([0,0,3])cylinder(h=4,d=5.5,$fn=30,center=true);
    }
}

module ObjimkapruchodkaProDiodu() {
    difference() {
        cylinder(h=2,d=7.5,$fn=30,center=true);
        cylinder(h=2,d=6.5,$fn=30,center=true);
    }
}


// spodni krabicka
module spodniKrabicka(){
    difference(){
        union() {
            spodniCast();
            translate([6,7.5,1]) sloupekSroubku();
            translate([6,48.5,1]) sloupekSroubku();
            
            translate([70,7.5,1]) sloupekSroubku();
            translate([70,48.5,1]) sloupekSroubku();
        }
        usbProstup();
    }
}

module sloupky() {
    translate([5.8,7.5,1]) sloupekSroubku();
    translate([5.8,48.5,1]) sloupekSroubku();
    
    translate([70,7.5,1]) sloupekSroubku();
    translate([70,48.5,1]) sloupekSroubku();    
}

module usbProstup(){
    color("Chartreuse")
    translate([72,22.5,7]) cube([6,14.5,7]);
    rotate([0,90,0]) {
        translate([-10.5,26,76.5]) roundedRect([8, 26, 1], 2, $fn=40);
    }
}

module usbVnitrniRantl(){
    difference() {
        translate([70,21.6,6]) cube([3,16.2,9]);
        translate([70,22.2,7]) cube([3,15,8]);
    }
}

module spodniCast(){
    difference() {
        cube([76,56,17]);
        translate([2,2,1]) cube([72,52,17]);
    }
}

module sloupekSroubku() {
    color("MediumVioletRed")
    difference() {
        cylinder(h=(4),d=(7),$fn=40,center=false);
        sroubek();
    }
}

module sroubek() {
    hlavickaSroubku();
    teloSroubku();
}

module hlavickaSroubku() {
    color("DeepPink")
    translate([0,0,19]) cylinder(h=4,d=6,$fn=30,center=false);
}

module teloSroubku() {
    color("HotPink")
    cylinder(h=19,d=3,$fn=30,center=false);
}

// size - [x,y,z]
// radius - radius of corners
module roundedRect(size, radius)
{
	x = size[0];
	y = size[1];
	z = size[2];

	linear_extrude(height=z)
	hull()
	{
		// place 4 circles in the corners, with the given radius
		translate([(-x/2)+(radius/2), (-y/2)+(radius/2), 0])
		circle(r=radius);
	
		translate([(x/2)-(radius/2), (-y/2)+(radius/2), 0])
		circle(r=radius);
	
		translate([(-x/2)+(radius/2), (y/2)-(radius/2), 0])
		circle(r=radius);
	
		translate([(x/2)-(radius/2), (y/2)-(radius/2), 0])
		circle(r=radius);
	}
}
