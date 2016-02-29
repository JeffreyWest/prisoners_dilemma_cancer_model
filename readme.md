# Prisoner's Dilemma as a cancer model
### Author: Jeffrey West, Date: 2 - 29 - 2016
## Publications
This code or code developed on top of this framework has been used in the following publications:

[The Prisoner's Dilemma as a cancer model](http://arxiv.org/abs/1512.04591)

[An evolutionary model of tumor cell kinetics and the emergence of molecular heterogeneity driving Gompertzian growth](http://arxiv.org/abs/1512.04590)

### How to use:
This code is written for use in MatLab, main.m is the executable file and buildA.m, buildQ.m, checkMutation.m, chooseBirthType.m, chooseDeathType.m are functions used by the main.m executable.

##### Parameters
The code runs an evolutionary dynamics simulation, which is a constant population process (N = # of cells competing = constant). The selection pressure, w, varies between 0 (no selection, equivalent to the Moran Process) and 1 (full selection pressure: maximum effect of the Prisoner's Dilemma).

This simple model encompasses two phenotypes (healthy, cancer) which consist of 2^b genetic types in total, where b is the bit length of the 'numerical genome' of each cell. For example, for b = 4, there are 16 genetic types which are course-grained into 11 healthy and 5 cancer types. 

![Phenotype - Genotype Mapping](https://github.com/JeffreyWest/prisoners_dilemma_cancer_model/blob/master/genotype_phenotype_mapping.png "Phenotype - Genotype Mapping")
