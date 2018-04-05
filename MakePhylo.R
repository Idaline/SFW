makePhylo=function(NewTaxo,MakePhyloO=TRUE)
{

	library(ade4)
	library(ape)
	library(taxize)

##### New data


NewTax=data.frame(Genus=NewTaxo$Genus,Family=NewTaxo$Family,Order=NewTaxo$Order,Class=NewTaxo$Class,Phylum=NewTaxo$Phylum,Kingdom=NewTaxo$Kingdom)
	row.names(NewTax)=NewTaxo$Num

	
##### 

	if(MakePhyloO==FALSE)
	{
		load('data/taxoAll.RData')
		row.names(taxT)=taxT$Num
		taxT=taxT[,-(1:2)]
		OldTax=taxT
		
	} else
	{
	
	load('data/data.RData')	
	load('data/taxoInc.RData')	

	
	Dig=data.frame(data$consD)	
	BibR=data.frame(data$ressB)
	BibC=data.frame(data$consB)
	
	
	Dig=Dig[,c(1,11:13)]
	Bib=rbind(BibR[,c(1,7:9)],BibC[,c(1,11:13)])
	TT=rbind(Dig,Bib)
	Bib$Kingdom[which(Bib$Phylum=='Euglenozoa')]='Protozoa'
	
	TTNtax=subset(TT,Genus=="Plantae" | Genus=="Fungi" | Genus=="Bacteria" | Genus=="Protozoa" | Genus=="Nematoda" | Genus=="Larve" | Genus=="Algae" | Genus=="Detritus"  | Genus=="Yeast" |  Genus=="lichen" | Genus=="Lichen")
	TTtax=subset(TT,Genus!="Plantae" & Genus!="Fungi" & Genus!="Bacteria" & Genus!="Protozoa" & Genus!="Nematoda" & Genus!="Larve" & Genus!="Algae" & Genus!="Detritus" & Genus!="Yeast" & Genus!="lichen" & Genus!="Lichen")
	
	TTtaxA=subset(TTtax,Kingdom=='Animalia')
	TTtaxF=subset(TTtax,Kingdom=='Fungi')
	TTtaxP=subset(TTtax,Kingdom=='Plantae')
	TTtaxR=subset(TTtax,Kingdom=='Protozoa' | Kingdom=='Bacteria')

	IdA=get_boldid(TTtaxA$Genus, division = "Animals")
	IdF=get_boldid(TTtaxF$Genus, division = "Fungi")
	IdP=get_boldid(TTtaxP$Genus, division = "Plants")
	IdR=get_boldid(TTtaxR$Genus)
	
	pA=cbind(classification(IdA[which(!is.na(IdA))],db="bold"))
	pF=cbind(classification(IdF[which(!is.na(IdF))],db="bold"))
	pP=cbind(classification(IdP[which(!is.na(IdP))],db="bold"))
	pR=cbind(classification(IdR[which(!is.na(IdR))],db="bold"))
	
pAA=data.frame(Family=pA$family,Order=pA$order,Class=pA$class,Phylum=pA$phylum,id=pA$query)
pFF=data.frame(Family=pF$family,Order=pF$order,Class=pF$class,Phylum=pF$phylum,id=pF$query)
pPP=data.frame(Family=pP$family,Order=pP$order,Class=pP$class,Phylum=pP$phylum,id=pP$query)
pRR=data.frame(Family=pR$family,Order=pR$order,Class=pR$class,Phylum=pR$phylum,id=pR$query)
	p=rbind(pAA,pFF,pPP,pRR)
	
	taxA=data.frame(TTtaxA,IdA)
	taxA=taxA[,1:5]
	taxP=data.frame(TTtaxP,IdP)
	taxP=taxP[,1:5]
	taxF=data.frame(TTtaxF,IdF)
	taxF=taxF[,1:5]
	taxR=data.frame(TTtaxR,IdR)
	taxR=taxR[,1:5]
	
	tax=rbind(taxA,taxF,taxP,taxR)
	
	taxP=merge(p,tax,by.x="id",by.y="ids")
	taxP=taxP[,c(6,7,2,3,4,5,8)]
	IncT=IncT[,c(3,4,8,7,6,5,1)]
	taxP=taxP[which(taxP$Genus=="Leitneria"),2:7]=NA
	
	if(length(which(!is.na(match(IncT$Num,taxP$Num))))>0)
	{
		IncT=IncT[-(which(!is.na(match(IncT$Num,taxP$Num)))),]
	}
	taxT=rbind(taxP,IncT)
	taxT$Family=as.character(taxT$Family)
	taxT$Order=as.character(taxT$Order)
	taxT$Class=as.character(taxT$Class)
	taxT$Order[which(is.na(taxT$Order))]=taxT$Class[which(is.na(taxT$Order))]
	taxT$Family[which(is.na(taxT$Family))]=taxT$Order[which(is.na(taxT$Family))]
	taxT$Class[which(taxT$Order=="Centramoebida")]="Discosea"
	taxT=unique(taxT)
	row.names(taxT)=taxT$Num
	taxT=taxT[,(-which(names(taxT) %in% "Num"))]	
	
TTNtax2=data.frame(Genus=TTNtax$Genus,Family=TTNtax$Family,Order=TTNtax$Order,Class=TTNtax$Class,Phylum=TTNtax$Phylum,Kingdom=TTNtax$Kingdom)
	row.names(TTNtax2)=TTNtax$Num
	OldTax=rbind(taxT,TTNtax2)	
	}	
	############### Distance phylo
	
	TT3=rbind(OldTax,NewTax)
	TT3$Order=as.character(TT3$Order)
	TT3$Order[which(TT3$Order=="Mesostigmata")]="Parasitiformes"
	TT3$Family=as.factor(TT3$Family)
	TT3$Order=as.factor(TT3$Order)
	TT3$Class=as.factor(TT3$Class)
	
	TT3=droplevels(TT3)
	tax=as.taxo(TT3)
	t=dist.taxo(tax)
	
	Pcoa2=dudi.pco(t,scannf=FALSE,nf=2)
	axes=data.frame(Pcoa2$l1)
	axesN=axes[which(!is.na(match(row.names(axes),row.names(NewTax)))),]
	axesO=axes[which(is.na(match(row.names(axes),row.names(NewTax)))),]
	axes=list(axesN,axesO)
	names(axes)=c('YourData','ParamData')
	return(axes)
}