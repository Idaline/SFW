#'Creates a dataframe species interactions
#'
#' @descSp Matrix with species traits (column names: Num, Bodymass, optional:CN) and taxonomy (column names: Kingdom, Phylum, Class, Order, Family). 
#' @axes a list with the 2 dataframes with species as rows, their names in the row.names, and their scores on the 2 first axes of the Pcoa of their taxonomy, created by 'MakePhylo'.
#' @DoParam if TRUE parametrizes the randomForest before making the prediction, to do if you used MakePhyloO=TRUE.
#' @return a matrix containing in order a column with consumers name, a column with resources name, and a column with the interactions between them (1 or 0). 
#' @export
predictInter=function(descSp,axes,DoParam=FALSE)
{


	axesN=axes$YourData
	
	TrTx=merge(descSp,axesN,by.x="Num",by.y="row.names")
	TrTx$Web[which(is.na(TrTx$Web))]=0
	
	TrTx$LogBM=log(TrTx$Bodymass+1)
	TrTx$LogBM[is.na(TrTx$LogBM)]=0
	
	TrTx$Web=as.factor(TrTx$Web)
	TrTx$Poison=as.factor(TrTx$Poison)
	TrTx$Herbivore =as.factor(TrTx$Herbivore)
	TrTx$Carnivore=as.factor(TrTx$Carnivore)
	TrTx$Fungivore=as.factor(TrTx$Fungivore)
	TrTx$Detritivore=as.factor(TrTx$Detritivore)
	TrTx$Aboveground=as.factor(TrTx$Aboveground)
	TrTx$Belowground=as.factor(TrTx$Belowground)
	TrTx$Toughness=as.factor(TrTx$Toughness)
	TrTx$Mobility=as.factor(TrTx$Mobility)
	
	TrR=data.frame(Resource=TrTx$Num,Bodymass.x=TrTx$LogBM,Toughness=TrTx$Toughness, Mobility=TrTx$Mobility,Aboveground.x=TrTx$Aboveground,Belowground.x=TrTx$Belowground,Kingdom=TrTx$Kingdom,RS1.x=TrTx$RS1,RS2.x=TrTx$RS2)
	
	TrC=data.frame(Consumer=TrTx$Num,Bodymass.y=TrTx$LogBM,Web=TrTx$Web,Poison=TrTx$Poison,Carnivore=TrTx$Carnivore,Fungivore=TrTx$Fungivore,Herbivore=TrTx$Herbivore,Detritivore=TrTx$Detritivore,Aboveground.y=TrTx$Aboveground,Belowground.y=TrTx$Belowground,RS1.y=TrTx$RS1,RS2.y=TrTx$RS2)
	
	TrR=unique(TrR)
	TrC=unique(TrC)
	d=rbind(TrR,TrR)
	for (i in 2:nrow(TrC))
	{
		d=rbind(d,TrR)
	}
	d=arrange(d,Resource)
	TrRC=data.frame(d,TrC)
	TrRC=unique(TrRC)

########### Données

	if(DoParam==TRUE)
	{

		consD=data$consD
		ressD=data$ressD
		consD=consD[,c(1:10)]
		ressD=ressD[,c(1:6,8)]
		
		consB=data$consB
		ressB=data$ressB
		consB=consB[,c(1:10)]
		ressB=ressB[,c(1:6,8)]
		
		ressDB=rbind(ressB,ressD)
		consDB=rbind(consB,consD)
		
		axesO=axes$ParamData
		ressDB=merge(ressDB,axesO,by.x="Num",by.y="row.names")
		consDB=merge(consDB,axesO,by.x="Num",by.y="row.names")
		
		interD=data$interD
		interB=data$interB
		interD=unique(interD[,2:4])
		interBD=rbind(interD,interB)
		
		dataPar=merge(interBD,ressDB,by.x="Resource",by.y="Num")
		dataPar=merge(dataPar,consDB,by.x="Consumer",by.y="Num")
		
		dataPar$Web=as.factor(dataPar$Web)
		dataPar$Poison=as.factor(dataPar$Poison)
		dataPar$Herbivore =as.factor(dataPar$Herbivore)
		dataPar$Carnivore=as.factor(dataPar$Carnivore)
		dataPar$Fungivore=as.factor(dataPar$Fungivore)
		dataPar$Detritivore=as.factor(dataPar$Detritivore)
		dataPar$Aboveground.x=as.factor(dataPar$Aboveground.x)
		dataPar$Belowground.x=as.factor(dataPar$Belowground.x)
		dataPar$Toughness=as.factor(dataPar$Toughness)
		dataPar$Mobility=as.factor(dataPar$Mobility)
		dataPar$Aboveground.y=as.factor(dataPar$Aboveground.y)
		dataPar$Belowground.y=as.factor(dataPar$Belowground.y)
		
		dataPar$Bodymass.x=log(dataPar$Bodymass.x+1)
		dataPar$Bodymass.y=log(dataPar$Bodymass.y+1)
				
		dataParCarn=subset(dataPar,(Kingdom=="Animalia" | Kingdom=="Bacteria" | Kingdom=="Protozoa") & Carnivore==1)
		dataParCarn$Omni=0
		dataParCarn$Omni[which(dataParCarn$Detritivore==1 | dataParCarn$Fungivore==1 | dataParCarn$Herbivore==1)]=1
		dataParCarn=dataParCarn[,(-which(names(dataParCarn) %in% c("Carnivore","Herbivore","Fungivore","Detritivore")))]	
		dataParCarn=dataParCarn[,-(1:2)]
		dataParCarn=unique(dataParCarn)
		dataParCarn=droplevels(dataParCarn)
		rfC=randomForest(Interac~.,dataParCarn)
	}
	else{
	
		rfC=rfC_sv
	}

#Carnivores

	interC2=subset(TrRC,Kingdom=="Animalia" | Kingdom=="Bacteria" | Kingdom=="Protozoa")
	interC2=subset(interC2,Carnivore==1)
	interC2$Omni=0
	interC2$Omni[which(interC2$Detritivore==1 | interC2$Fungivore==1 | interC2$Herbivore==1)]=1

	
	#should be Consumer -> .y and Resource -> .x
		
	pC=predict(rfC, newdata=interC2,predict.all=FALSE)	
	interC2$pred=pC

#HFD

	interHFD=subset(TrRC,Kingdom!="Animalia" & Kingdom!="Bacteria" & Kingdom!="Protozoa")
	interHFD$pred=0
	interHFD$pred[which(interHFD$Fungivore==1 & interHFD$Kingdom=="Fungi")]=1
	interHFD$pred[which(interHFD$Herbivore==1 & interHFD$Kingdom=="Plantae")]=1
	interHFD$pred[which(interHFD$Detritivore==1 & interHFD$Kingdom=="Detritus")]=1
	
	#All
	
	interPredHFD=data.frame(Consumer=interHFD$Consumer,ress=interHFD$Resource,interac=interHFD$pred))
	interPredC=data.frame(Consumer=interC2$Consumer,Resource=interC2$Resource,interac=interC2$pred)
	
	inter=unique(rbind(interPredHFD,interPredC))
	
	return(inter)
}