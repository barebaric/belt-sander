# Mostly Printed Edge Belt Sander

![Version 1 Prototype](media/sander-v1.jpg)

## Project Goals And Design Constraints

In this project I am designing and building an edge belt sander. Design constraints:

* Using only 3D printed components + widely available off-the-shelf hardware (as far as possible) to make it fast and easy to reproduce.

* For belt size 915 x 100 mm.

* Speed controllable brushless motor.

* Belt change and alignment adjustment with no tools.
- Low profile form factor of less than 14cm - the sander should fit into a drawer when not in use.
* Modular design to allow for customization and faster/cheapter iteration during development.

* Ideally: Option to power from two 18V batteries.

## Overall Status

**V1 is a rough, first monolithic prototype** - probably not at all usable without lots of manual tweaking. It was not even fully tested - I only printed it and assembled without a motor to find issues with the overall design.

**V2 is work in progress** and completely unusable so far.

## Checklist And Roadmap For First Release

- [x] Prototype a rough full model to ensure the dimensions fit
  
   - [Version 1](v1)

- [x] Design, test, and refine the lever mechanism for the belt tightener
  
   - Done after testing more than 20 different prototypes ([type 1](disc lever), [type 2](slotblock lever), [type 3](rotating lever))
   - [Latest iteration, successfully printed and tested](mechanisms/push%20lever)
     ![Lever designs](media/levers.jpg)

- [x] Choose a motor
  
   - Dismissed: [997 motor](components/997%20motor), mostly due to ventilation requirements.
   - Dismissed: [Skateboard motor type 1](components/90%20mm%20skateboard%20motor). Doesn't fit into desired envelope (see design constraints above). It is too long with the shaft included.
   - Dismissed: [6384 motor](components/6384%20motor). Just about fits, but with no space left for routing the cable securely and more expensive because it requires large bearings.
   - **Chosen**: [90 mm skateboard motor type 2](components/90%20mm%20skateboard%20motor%20(flat%20type))
     ![Skateboard motor](components/90%20mm%20skateboard%20motor%20(flat%20type)/motor.jpg)

- [x] Choose a BLDC controller
  
   - Dismissed: SNR8503M BLDC controller module for insufficient documentation. Is is also too close to the motor power limit, so may be risky to use anyway.
     ![SNR8503M module](media/SNR8503M-BLDC-controller-module.jpg)
  
   - Dismissed: ZS-X11D1 module. Power limit too low & documentation insufficient.
     
     ![ZS-X11D1](media/ZS-X11D1 module.jpg)
  
   - **Chosen**: MKS XDrive (ODrive) Mini ![MKS XDrive (Odrive) Mini](electronics/odrive-mini/odrive.jpg)

- [ ] Create CAD models for all electronic components
  
   - [ ] [90 mm skateboard motor](components/90%20mm%20skateboard%20motor%20(flat%20type)): **Draft done**, but some dimensions guessed for lack of technical drawings from the supplier. Will need updating once the parts arrive.
  
   - [x] [MKS XDrive (Odrive) Mini](electronics/odrive-mini)
  
   - [ ] Brake resistor
  
   - [ ] Potentiometer
  
   - [ ] Power switch
  
   - [ ] Microcontroller
  
   - [ ] Fan (if needed based on testing)
  
   - [ ] Makita battery adapter

- [ ] Build and test the electronics
  
   - [x] Order parts
  
   - [ ] Configure and test ODrive
  
   - [ ] Program microcontroller with potentiometer

- [ ] Design and refine an electronics enclosure module to fit into the sander
  
   - [ ] Enclosure
  
   - [ ] Potentiometer knob
  
   - [ ] Perhaps include a display for speed?
  
   - [ ] Final assembly

- [ ] Design and refine hub driven roller with the skateboard motor.
  
   - [ ] [Model drafted](hub with skateboard motor (flat type)), but some dimensions guessed for lack of technical drawings of the motor. Will need updating after finalizing the motor model.
   - [ ] Depending on heat development, the hub could be made slightly smaller to make the belt unit wider.

- [ ] Create the new overall assembly tying everything together.
  
   - [ ] Create the design.
  
   - [ ] Print, test, refine, iterate

- [ ] Make a backing plate from steel or aluminium.

- [ ] Design and build the final base plate.

## Incorporate lessons learned from V1 prototype

- [ ] Provide more clearance (1.5mm on each side?) between rollers and housing, to fit washers on both sides.

- [ ] Make left carriage bearing cutouts larger - 23mm probably. It doesn't need tight clearance, as it is pulled to the right anyway.

- [ ] Separate the carriage arm from the bracket, make them connectable using screws. For better printability/modularity.

- [ ] Make the arm shorter, to make more space for electronics in the unit.

- [ ] Recess the tightener knob into the unit.

- [ ] 
- [ ] Bottom could be 2mm slimmer because it will be seated in a base plate anyway.

## Potential extensions after V2

* Dust extraction

* Speed display

* Oscillating unit

* Reduce velocity based on proximity sensor
