
spodniKrabicka();


// spodni krabicka
module spodniKrabicka(){
    difference(){
        union() {
            spodniCast();
            translate([5,6.5,1]) sloupekSroubku();
            translate([5,47.5,1]) sloupekSroubku();
            
            translate([69.5,6.5,1]) sloupekSroubku();
            translate([69.5,47.5,1]) sloupekSroubku();
        }
        usbProstup();
    }
}

module usbProstup(){
    color("Chartreuse")
    translate([72,22.5,7]) cube([3,14.5,7]);
}

module spodniCast(){
    difference() {
        cube([74.5,54,19]);
        translate([1,1,1]) cube([72,52,19]);
    }
    vyztuhyDna();
    vyztuhaSpodniKrabicky();
}

module vyztuhyDna() {
    color("HotPink")
    rotate([0,0,35]) {
        translate([1,0,0]) cube([89,1.5,1.5]);
        translate([15,20,0]) cube([48.4,1.5,1.5]);
        translate([29,-20,0]) cube([47.5,1.5,1.5]);
    }
    
    color("HotPink")
    rotate([0,0,150]) {
        translate([-60,-45,0]) cube([85,1.5,1.5]);
        translate([-40,-25,0]) cube([53,1.5,1.5]);
        translate([-48,-65,0]) cube([43,1.5,1.5]);
    }
}

module vyztuhaSpodniKrabicky() {
    color("SpringGreen")
    rotate([0,-12,0]) {
        translate([0.5,0,0]) cube([75.5,1.5,1.5]);
    }
    color("SpringGreen")
    rotate([0,-12,0]) {
        translate([0.5,52.5,0]) cube([75.5,1.5,1.5]);
    }
    color("SpringGreen")
    rotate([0,-15,90]) {
        translate([0.5,-1.5,0]) cube([55,1.5,1.5]);
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