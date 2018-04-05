library(randomForest)
library(plyr)

predictInter=function(descSp,axes)
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

load('data/data.RData')

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
interD$Consumer=paste(interD$Consumer,"DIG",sep=" ")
interD$Resource=paste(interD$Resource,"DIG",sep=" ")
interBD=rbind(interD,interB)

dataPar=merge(interBD,ressDB,by.x="Consumer","Num")
dataPar=merge(dataPar,consDB,by.x="Resource","Num")

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


dataParCarn=subset(dataPar,Kingdom=="Animalia" & Carnivore==1)
dataParCarn=dataParCarn[,(-which(names(dataParCarn) %in% c("Carnivore","Herbivore","Fungivore","Detritivore","Kingdom")))]	
rfC=randomForest(Interac~.,dataParCarn)

dataParHFD=subset(dataPar,Kingdom!="Animalia")
rfHFD=randomForest(Interac~.,dataParHFD)

#Carnivores

interC2=subset(TrRC,Kingdom=="Animalia")
interC2=subset(interC2,Carnivore==1)

interC2names=data.frame(ress=interC2$Resource,cons=interC2$Consumer)

pC=predict(rfC, newdata=interC2,predict.all=FALSE)

interC2$pred=pC
interCnames=cbind(interC2names,interC2)

#HFD

interHFD=subset(TrRC,Kingdom!="Animalia")
interH=subset(interHFD,Herbivore==1)
interF=subset(interHFD,Fungivore==1)
interD=subset(interHFD,Detritivore==1)

interHFD2=unique(rbind(interH,interF,interD))

interHFD2names=data.frame(ress=interHFD2$Resource,cons=interHFD2$Consumer)

pHFD=predict(rfHFD, newdata=interHFD2,predict.all=FALSE)

interHFD2$pred=pHFD
interHnames=cbind(interHFD2names,interHFD2)

#All

interPredHFD=data.frame(cons=interHnames$cons,ress=interHnames$ress,interac=interHnames$pred)
interPredC=data.frame(cons=interCnames$cons,ress=interCnames$ress,interac=interCnames$pred)

inter=rbind(interPredHFD,interPredC)

return(inter)
}



###########

