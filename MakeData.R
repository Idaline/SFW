consB=read.table("~/Documents/Ecole/These/BdDInterac/DataFinal/Bib/consBib.csv",head=TRUE,sep=";",row.names=1)
ressB=read.table("~/Documents/Ecole/These/BdDInterac/DataFinal/Bib/ressBib.csv",head=TRUE,sep=";")
interB=read.table("~/Documents/Ecole/These/BdDInterac/DataFinal/Bib/interBib.csv",head=TRUE,sep=",",row.names=1)

consD=read.table("~/Documents/Ecole/These/BdDInterac/DataFinal/Dig/conso.csv",head=TRUE,sep=";")
ressD=read.table("~/Documents/Ecole/These/BdDInterac/DataFinal/Dig/resso.csv",head=TRUE,sep=";")
interD=read.table("~/Documents/Ecole/These/BdDInterac/DataFinal/Dig/interac.csv",head=TRUE)

consD=consD[,c(1:4,6,7,5,8,9,10,11,16,17)]
consB=consB[,c(1,9:17,7,2,8)]
ressB=ressB[,c(1,9:13,7,2,8)]
ressD=ressD[,c(1,2:7,12,13)]

colnames(ressB)[1]="Num"
colnames(ressB)[2]="Bodymass"
colnames(ressB)[3]="Toughness"
colnames(ressB)[5]="Aboveground"
colnames(ressB)[6]="Belowground"
colnames(ressD)=colnames(ressB)

colnames(consB)[1:2]=c("Num","Bodymass")
colnames(consB)[9:10]=c("Aboveground","Belowground")
colnames(consD)=colnames(consB)

consD$Num=paste(consD$Num,"DIG",sep=" ")
ressD$Num=paste(ressD$Num,"DIG",sep=" ")

data=list(consD,ressD,interD,consB,ressB,interB)
names(data)=c("consD","ressD","interD","consB","ressB","interB")
save(data,file="~/Documents/Ecole/These/PkgFW/data/data.RData")