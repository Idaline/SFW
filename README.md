# SFW
R package for modeling soil food web dynamics and predicting species interactions from their traits

## MakePhylo

Gives you the species scores on the pcoa of taxonomic distances. You need to provide a data frame with your species as rows, and their taxonomy as columns (Kingdom, Phylum, Class, Order, Family and Genus). Species taxonomic distances are calculated according to a parametrization data. Because of the variability in invertebrate classification, it is advised to determine their taxonomy with the package taxize, and the database "bold". If you want to be sure that the taxonomy of the parametrization data is up to date you can update it with MakePhyloO=TRUE, but it takes time. 

## PredictInter

Gives you a data frame with species interactions calculated according to parametrization data and species traits. Traits should be named Bodymass, Mobility, Toughness, Web, Poison, Aboveground, Belowground and are described in Laigle I, Aubin I, Digel C, Brose U, Boulangeat I and Gravel D (2018), \textit{Species traits as drivers of food web structure}, Oikos 127: 316-326. If you updated the parametrization data taxonomy you need to parametrize again the predictive model with DoParam=TRUE, which can take a few hours. 

##CreateSource

Creates text files which are dataframes with species parameters and with species preferences needed to run the model. You should provide a dataframe with species taxonomy (Kingdom, Phylum, Class, Order, Family) and species bodymass, and optionally species C:N.

##RunModel

Run the dynamic model of your community with the files created by "CreateSource" and a vector containing the ending time (suggested:100), the time step (suggested:0.01), the N input, the N output (\%), the detritus input (e.g. from trees), the detritus input C:N. Creates a text file with species biomass in function of time. 

