



rezy();

module rezy() {
    difference() {
        vrchniKrabicka();
        
        // odstrani vrchni casti
        //translate([0,-15,11]) cube([170,100,22]);
        
        // odstrani stredni cast
        //translate([0,-15,0]) cube([170,100,11]);
        
        // dstrizeni spodni casti 
        translate([0,-15,-10]) cube([170,100,10]);
    }
    //spodniKrabicka();
}

module spodniKrabicka() {
    difference() {
        translate([0,0,-6]) cube([162,64,6]);
        translate([1,1,-4]) cube([160,62,4]);
    }
}

module vrchniKrabicka() {
    color("lightblue")

    difference(){
        
        difference() {
            cube([162,64,33]);
            translate([1,1,0]) cube([160,62,32]);
        }
        
        translate([0,64,0]) rotate(a=[180,180,90]) odkrytiOvladacihoPanelu(51,13,24);
        
        
        // odvetrani krabicky
        odvetravani(30);
        
        // svorkovnice
        translate([153,51,0]) vstupniSvorka();
        translate([153,1,0]) svorkaRele();
        
        // prostupky
        rotate(a=[90,90,270]) translate([-10,-21,-163]) prostupky();
        rotate(a=[90,90,270]) translate([-10,-58,-163]) prostupky();
        
    }

    // celo odkryti panelu
    translate([13,62,0]) rotate(a=[180,180,90]) odkrytiOvladacihoPanelu(50,1,23);
    
    // bocnice odkryti panelu
    translate([0,13,0]) rotate(a=[180,180,90]) odkrytiOvladacihoPanelu(1,13,22);
    
    translate([0,64,10]) rotate(a=[180,180,90]) ovladaciPanel();
    
    // vzpery 
    rotate(a=[180,0,270]) translate([-20,-19,-27]) vzpera();
    rotate(a=[180,0,270]) translate([-35,-19,-27]) vzpera();
    rotate(a=[180,0,270]) translate([-55,-19,-27]) vzpera();
    
    rotate(a=[90,0,180]) translate([-10.5,27,6]) vzpera();
    rotate(a=[90,0,180]) translate([-25,27,6]) vzpera();
    rotate(a=[90,0,180]) translate([-140,27,6]) vzpera();
    rotate(a=[90,0,180]) translate([-150,27,6]) vzpera();
    
    rotate(a=[180,0,180]) translate([-20,58,-27]) vzpera();
    rotate(a=[180,0,180]) translate([-140,58,-27]) vzpera();
    rotate(a=[180,0,180]) translate([-150,58,-27]) vzpera();
    
    rotate(a=[90,0,270]) translate([-35,27,-156]) vzpera();
    rotate(a=[90,0,270]) translate([-45,27,-156]) vzpera();
   
    
    
    // sroubky prava strana
    translate([20,-5.5,-6]) rotate(a=[0,0,90]) sloupekSroubku();
    translate([145,-5.5,-6]) rotate(a=[0,0,90]) sloupekSroubku();
    
    // sroubky leva strana
    translate([20,69.5,-6]) rotate(a=[0,0,270]) sloupekSroubku();
    translate([145,69.5,-6]) rotate(a=[0,0,270]) sloupekSroubku();
    
}


module ovladaciPanel() {
    union() {
        difference() {
            color("YellowGreen") cube([51,13,1]);
            translate([9,6,0]) dioda();
            translate([17,6,0]) dioda();
            translate([24,6,0]) dioda();
            translate([32,6,0]) dioda();
            translate([43,7.5,0]) nozickaTlacitka();
        }
        
        
        //rotate(a=[0,180,90]) translate([-13,-6,5]) vzpera();
        rotate(a=[90,0,180]) translate([-13.5,-5,6]) vzpera();
        rotate(a=[90,0,180]) translate([-21,-5,6]) vzpera();
        rotate(a=[90,0,180]) translate([-39,-5,6]) vzpera();
        rotate(a=[90,0,180]) translate([-49,-5,6]) vzpera();
    }
}


// pro vyztuzeni
module vzpera() {
    color("Lime")
    rotate(a=[0,90,0])
    difference()  {
        cube([5,5,1]);
        translate([0,0,-1]) cylinder(h=3, d=10, center=false, $fn=30);
    } 
}

module odkrytiOvladacihoPanelu(x, y, z) {
    translate([-1,0,10]) color("DeepSkyBlue") cube([x,y,z]);
}

module dioda() {
    color("OrangeRed")
    cylinder(h=12, d=5.5, center=false, $fn=30);
}

module nozickaTlacitka() {
    color("LightSeaGreen")
    cylinder(h=12, d=4, center=false, $fn=30);
}

module vstupniSvorka() {
    color("lightgreen")
    cube([9,12,34]);
    
}

module svorkaRele() {
    color("lightpink")
    cube([10,15,34]);
}


module sloupekSroubku() {
// h=37, d=15  
    color("MediumVioletRed")
    difference() {
        union() {
            cylinder(h=37,d=13,$fn=40,center=false);
            translate([0,-6.5,0]) cube([6.0,13,37]);
        }
        translate([0,0,2]) sroubek();
    }
}


module sroubek() {
    hlavickaSroubku();
    teloSroubku();
}

module teloSroubku() {
// telo 16x3    
    color("HotPink")
    cylinder(h=16,d=3,$fn=30,center=false);
}

module hlavickaSroubku() {
// hlavicka 3x5.5
    color("DeepPink")
    translate([0,0,16]) cylinder(h=20,d=6,$fn=30,center=false);
}

module odvetravani(start = 17) {
    
    // leva strana
    for (a =[25:0]){ 
        translate([start+a*4,63,7]) rotate(a=[180,180,90]) odkrytiOvladacihoPanelu(8,1,18);
    }
    
    // prava strana
    for (a =[25:0]){ 
        translate([start+a*4,7,7]) rotate(a=[180,180,90]) odkrytiOvladacihoPanelu(8,1,18);
    }
}

module prostupky() {
    color("RosyBrown")
    hull() {
       translate([0,15,0]) cylinder(r=3,h=3,$fn=30);
       cylinder(r=3,h=3,$fn=30);
    }    
}
