# Atom Simulator

## A Brief Description

**Atom Simulator** is an educational application designed for students who want to understand the fundamental principles of chemistry. The program allows users to simulate molecules, visualize the chemical bonds formed between atoms based on their valencies, and discover information about elements such as *oxygen*, *hydrogen*, *carbon*, and others.

## Chemistry Concepts

The current version of the application allows the user to visualize molecules containing 7 types of atoms (represented by circles of different colors and sizes):
- Carbon - Black
- Oxygen - Red
- Hydrogen - White
- Nitrogen - Blue
- Sulfur - Yellow
- Sodium (as an ion) - Pink
- Chlorine (as an ion) - Green

To use the simulator, the user must know a few important chemistry rules:
- A hydrogen atom accepts a maximum of 1 bond.
- An oxygen atom accepts a maximum of 2 bonds.
- A nitrogen atom accepts a maximum of 3 bonds.
- A carbon atom accepts a maximum of 4 bonds.
- A sulfur atom accepts a maximum of 6 bonds (sulfur functionality has been reduced in the context of this application).
- A sodium atom accepts a maximum of 1 bond.
- A chlorine atom accepts a maximum of 1 bond.

## Simulator Mechanics
The developed application is very intuitive. The user's goal is to create molecules using pre-set atoms and the bonds formed between them. The simulator's operation is exemplified below:
1) Select an atom type from the atom menu.
2) *Left-click* in the workspace to add atoms.
3) *Right-click* on atoms to select them.
   - To create a bond -> *Right-click* on another atom that has available electrons.
   - To delete a bond -> *Right-click* on the atom with which it shares the bond you want to delete.
   - To delete the selected atom -> Press the *Delete* key.
4) To reset the workspace -> Press the *R* key.
5) To close the simulator -> Press the *ESC* key.

Once all atoms in the workspace are **connected**, the program will display the name of the formed molecule and its molecular mass. If the created molecule is **recognized** by the application, it will also display its specific name.

## Other Functionalities
In addition to modeling various molecules, the simulator allows for two subtle functionalities:
- For carbon chains with double bonds => The geometric configuration type can be set (to change the configuration, *left-click* on the double bond).
- For molecules consisting only of single bonds => The *Play* button can be pressed to set the physics mode (single bonds rotate).

## GUI
The simulator has a minimalist GUI that allows any user to understand how the application works. Steps to follow (for those lacking the art of deduction):
1) Choose the desired resolution (currently only 2 resolutions are available, more in the future).
2) The user can choose to do whatever they wish from the simulator menu (it is recommended to press the *?* button).
3) Activate **Help Overlay** mode for an explanation of functionalities; press any key to exit.
4) Activate **Tutorial** mode for a short tutorial including the atoms featured in the simulator.
5) Activate **Sandbox** mode to understand functionalities; here the user can interact with atoms; if a created molecule corresponds to a real-world molecule, the simulator will specify this.
6) Activate **Trivia** mode to test accumulated knowledge; here the user is presented with several molecules to reproduce by name; each correct answer earns the user a point; upon exiting, an input box will appear for the name to be displayed on the leaderboard.
7) Activate **Leaderboard** mode to view the Top-10 best results recorded locally.

## Resources
The resources mentioned above do not belong to me. They were used to create a visually and audibly attractive application. Some of these resources were converted from their original format for compatibility reasons. The authors of these resources hold the copyright.

- [SFML](https://github.com/SFML/SFML/tree/3.0.2) (Zlib)
- **assets** directory (IMAGES)
  - **periodic-table.jpg**: [source](https://npr.brightspotcdn.com/legacy/sites/wuwm/files/201912/AdobeStock_238869971.jpg)
  - **menu-background.jpg**: [source](https://img.freepik.com/free-vector/science-logo-banner-with-lab-tools-equipment_1308-157213.jpg?semt=ais_hybrid&w=740&q=80)
- **assets** directory (SOUNDS)
  - **trivia-corect.wav**: [source](https://www.myinstants.com/media/sounds/kids-saying-yay-sound-effect_3.mp3)
  - **trivia-gresit.wav**: [source](https://www.myinstants.com/media/sounds/wrong-answer_L4HjrqG.mp3)
  - **click-selectare.wav**:
  - **click-stergere.wav**:
  - **TutorialMusic.ogg**: [source](https://www.youtube.com/watch?v=jj0ChLVTpaA)
  - **Leaderboard.ogg**: [source](https://www.myinstants.com/media/sounds/fnaf-1-music-box.mp3)
  - **MainMusic.ogg**: 
- **assets** directory (FONT)
  - **Roboto-VariableFont_wdth,wght.ttf**: [source](https://fonts.google.com/specimen/Roboto)
