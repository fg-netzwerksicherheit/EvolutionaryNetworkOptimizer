================================================================================================
Evolutionary Network Optimizer - Optimizing communication networks using evolutionary algorithms
================================================================================================

This is the repository of the Evolutionary Network Optimizer project, which is developed with the aim of diminishing the nightmare of network optimization. The project takes advantages of Evolutionary Computation and Network Simulator (NS3) to produce an optimal network using a plausible time and computing resource. 

Get the source on GitHub: https://github.com/fg-netzwerksicherheit/EvolutionaryNetworkOptimizer

Structure
=========

The whole project consists 4 sub-projects:

- OpenEA: A framework for efficient Evolutionary Computing (https://github.com/fg-netzwerksicherheit/openEA)
- OpenEA-GUI: the Graphical User Interface, which supports network graph and event visualization and real-time EA fitness plotting 
- Automated Simulations: the module developed to evaluate network via simulation, which supports to acquire simulation information and configurate network and event manually
- AnyGraph: the module used to import & export network graph file (*.p2pnetml, *.graphml) and import event file (*.json, *.xml)

The list below shows how these sub-projects are related to each other and which one requires which ones.

    NetworkOptimizer
        OpenEA
        Automated Simulations
            AnyGraph
        AnyGraph
    OpenEA-GUI
        OpenEA


Please follow the links below to read the installation guide, usage and documentation of each sub-project:

- OpenEA: <TODO: insert link to OpenEA directory, readme.rst or documentation>
- OpenEA-GUI: <TODO: insert link to OpenEA-GUI directory of documentation>
- Automated Simulations: <TODO: insert link to Automated Simulations directory of documentation>
	- Go to directory Simulation and type "doxygen Doxyfile" to generate html documents
	- Go to directory Simulation/Doxy-html/html and open index.html
- NetworkOptimizer:
	- Do the similar thing to the case of Automated Simulations
- AnyGraph: <TODO: insert link to AnyGraph directory of documentation>

Requirements
============

Operating System: Ubuntu 16.04.2 LTS (64-bit version) or equivalent


Disclaimer
==========

This project is not guaranteed to work on other operating systems. 

Authors
=======

- Robin Mueller-Bady
- Bui Quang Minh
- Kiet Quang Huynh Vo
- Chi-Thanh Nguyen 
- Nguyen-Quang Pham
- Manuel Schildknecht

Individual contribution is shown in the specific source files.
Please see https://netzwerksicherheit.fb2.fh-frankfurt.de/ for further details on the research group.


Copyright and License
=====================

Copyright 2017 Frankfurt University of Applied Sciences


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Acknowledgement
===============

This work was supported in the framework of Hessen ModellProjekte, financed with funds of the European Union (European Regional Development Fund - ERDF) and the State of Hessen in the context of the research project "Reactive network Optimization by Using SDN-Technology" (ROBUST) (HA project no. 473/15-15).
