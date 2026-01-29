# ModularPlantModeler







## Contents
1. [Modular Plant Modeler (Free Version)](#1-modular-plant-modeler-free-version)
3. [License Version (Advanced)](#3-license-version-advanced)


<p align="center">
<img src="https://github.com/RyuZhihao123/Modular-Tree-Modeler-25/blob/main/Figures/figure_gallery.png" width="70%">
</p>


## 1. Modular Plant Modeler (Free Version)

### 1.1. Introduction

Efficient generation of customized 3D plant models is an important problem in computer graphics. However, existing procedural methods always rely on algorithmically writing complex structural rules, thus very difficult and unintuitive for non-experts to learn.

This repository introduces a more advanced 3D modeling system, which enables users to **intuitively** manipulate a so-called **``Modular Graph``** to easily design and obtain diverse 3D plant models. 

📺 **[[Preview Video (1min)]](https://drive.google.com/file/d/1FugtnOsYm2L_HkJawwiUUKoe6aQs5e93/view?usp=sharing): Short demo video for fast preview. (Recommend watching it before starting🔥🔥!)**

📺 [[Full instruction Video (5min)]](https://drive.google.com/file/d/1uOwghzsHS2jrZXc8RHVIOitZZubseMT-/view?usp=sharing): A long instruction video to teach the usage step by step.

🟠 [[Program Download]](https://github.com/RyuZhihao123/Modular-Tree-Modeler-25/tree/main/GraphTreeModeler): Please download the entire folder to execute the program.

BTW, I am continuing adding new features to this software. If you have any suggestions or requirements, please feel free to raise an issue! ^_^.


### 1.2. Quick Start

📺 Before starting, we also strongly recommend that all users carefully watch this [[5-min Instruction Video]](https://drive.google.com/file/d/1uOwghzsHS2jrZXc8RHVIOitZZubseMT-/view?usp=sharing) to understand the step-by-step process of designing a simple tree species.

This software ([download link](https://github.com/RyuZhihao123/Modular-Tree-Modeler-25/tree/main/GraphTreeModeler)) is only runable on Windows PCs (we successfully tested on Win 10 and Win 11).


https://github.com/user-attachments/assets/f0d309b6-fb10-4030-9ec2-0a2231a6d7a9









### 🌳 [A] Create Trees using Existing Species Templates:

For easy start, we provided some species templates that you can directly load and see the modeling results.

(Step-0) Download the program from [this folder](https://github.com/RyuZhihao123/Modular-Tree-Modeler-25/tree/main/GraphTreeModeler).

(Step-1) Open the first program ``GraphTreeModeler.exe``, which is the window for visualization of 3D results.

(Step-2) Then open the second program ``GraphTreeModeler_UI.exe``, which is the core user interface for interactively designing species graphs. 

Please note that:
- The second exe often takes around 6 seconds for initialization. Please be patient to wait.
- **You must make sure to sequentially open two exes in this order.** They have dependencies thus cannot be opened in an inverse order.

(Step-3) For convenience, we've provided several pre-created species templates under this [folder path](https://github.com/RyuZhihao123/Modular-Tree-Modeler-25/tree/main/GraphTreeModeler/SavedGraphs), which you can use for quickly creating some 3D tree models. 

- To load them, go to the **``Menu Bar``** of the second program (i.e., ``GraphTreeModeler_UI``), and click **``File``** -> **``Load Graph``**. Then, after selecting a template file and pressing the ``Generate New`` button, our system will automatically generate a nice-looking 3D tree within half a second.
- You can also refer to the **[[Preview Video]](https://drive.google.com/file/d/1FugtnOsYm2L_HkJawwiUUKoe6aQs5e93/view?usp=sharing)** to see how to load the templates. **(Recommend!)**

### 🌳 [B] Design Your Own Species From Scratch:


You are also allowed to freely design your own species! Please watch the [[5-min Instruction Video]](https://drive.google.com/file/d/1uOwghzsHS2jrZXc8RHVIOitZZubseMT-/view?usp=sharing) to see how to practically use the system.

FYI, the main operations and hotkeys are listed below:

- ``Left MouseButtion``: Select/move an existing node.
- ``Ctrl`` + ``Left MouseButtion``: Add a new node.
- ``Alt`` + ``Left MouseButtion``: Connect edges between two nodes.
- ``Backspace``: Delete a selected node.

<p align="center">
  <img src="https://raw.githubusercontent.com/RyuZhihao123/Modular-Tree-Modeler-25/main/Figures/interfaces.png"
       alt="Interfaces"
       width="700">
  <br>
  <em>Figure 1. The user interface.</em>
</p>


### 1.3 Diverse Data Modalities are supported
Our system allows for automatically exporting diverse data modalities. Some are almost impossible for humans to manually annotated (e.g., skeletons, and leaf masks).
<p align="center">
  <img src="https://raw.githubusercontent.com/RyuZhihao123/Modular-Tree-Modeler-25/main/Figures/fig_representation.png"
       alt="Data representation">
  <br>
  <em>Figure 2. Example data modalities supported by our system.</em>
</p>




## 3. License Version (Advanced)

- **The above version is totally for Free Download and Experience**, which includes most of core features for designing the structure of 3D tree models.

- **In addition, we have also developed an ``advanced version`` for more professional species design**, which supports more enhanced capabilities beyond modular graphs, including but no limited to (as shown in the following figures:
```
  - Deeper paramatric control of more botanical phenomenons (e.g., Apical Domaninace, Gravity, Light competition, Shade, etc).
  - Better support for smaller farm/house plants
  - Flexible control of leaf structures.
  - 3D Fruit Modeling (e.g., Apples, Pears, Grapes, Tomatos).
  - A more interactive user interface.
  - Forest-level scene creation.
```

🎈 If you're interested in experience, commercial use, or even research collabration, please directly reach me out via email (liuzh96@outlook.com).
This enhanced version is a very important property for us to do exclusive research (and we made money on it), thus a license is required for outsiders to use.


<p align="center">
  <img src="https://raw.githubusercontent.com/RyuZhihao123/Modular-Tree-Modeler-25/main/Figures/figure_advanced_features.png"
       alt="Advanced features">
  <br>
  <em>Figure 3. Advanced features of the license-required modular plant modeling system.</em>
</p>

<p align="center">
  <img src="https://raw.githubusercontent.com/RyuZhihao123/Modular-Tree-Modeler-25/main/Figures/fig_multi.png"
       alt="Forest generation">
  <br>
  <em>Figure 4. Forest generation and its corresponding modalities.</em>
</p>



