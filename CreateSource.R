library(plyr)

CreateSource=function(inter,descSp,name,setPref=TRUE)
{
########## Param
	Prod=nrow(subset(descSp,Kingdom=="Plantae"))
	Mic=nrow(subset(descSp,Kingdom=="Bacteria" || Kingdom=="Fungi"))

	hrc=descSp
	if(is.null(hrc$CN))
	{
		hrc$CN=0
	
		hrc$CN[which(hrc$Kingdom=="Protozoa")]=7
		hrc$CN[which(hrc$Phylum=="Arthropoda")]=8
		hrc$CN[which(hrc$Phylum=="Annelida")]=5
		hrc$CN[which(hrc$Phylum=="Cyanobacteria")]=20
		hrc$CN[which(hrc$Phylum=="Mollusca")]=4.5
		
		hrc$CN[which(hrc$Phylum=="Nematoda")]=8.8
		hrc$CN[which(hrc$Order=="Sarcoptiformes")]=5
		hrc$CN[which(hrc$Order=="Parasitiformes")]=2.7
		hrc$CN[which(hrc$Order=="Coleoptera")]=6.5
		hrc$CN[which(hrc$Family=="Ptiliidae")]=7.8
		hrc$CN[which(hrc$Order=="Entomobryomorpha")]=7.3
		hrc$CN[which(hrc$Order=="Neelipleona")]=7.7
		hrc$CN[which(hrc$Order=="Poduromorpha")]=12.3
		hrc$CN[which(hrc$Order=="Symphypleona")]=4.9
		hrc$CN[which(hrc$Order=="Julida")]=6.5
		hrc$CN[which(hrc$Family=="Enchytraeidae")]=5.0
		hrc$CN[which(hrc$Class=="Gastropoda")]=8.7
		hrc$CN[which(hrc$Family=="Araneae")]=5.4
		hrc$CN[which(hrc$Order=="Isopoda")]=4.3
		hrc$CN[which(hrc$Order=="Pseudoscorpionida")]=4.5 
	}
	
	hra=hrc
	hra=unique(hra)
	
	hra$type=NA
	hra$type=as.character(hra$type)
	hra$biomass=1
    hra$type[which(hra$Kingdom=="Animalia"  | hra$Kingdom=="Protozoa")]="Consumer"
	hra$type[which(hra$Kingdom=="Plantae")]="Producer"
	hra$type[which(hra$Kingdom=="Bacteria" | hra$Kingdom=="Fungi")]="Microbes"
	hra$type[which(hra$Kingdom=="Detritus")]="Detritus"

	if(is.null(hra$ax))
	{	
		hra$ax=0
		hra$ax[which(hra$type=="Consumer")]=0.314
	}
	if(is.null(hra$y))
	{	
		hra$y=0
		hra$y[which(hra$type=="Consumer")]=8
	}
	if(is.null(hra$r))
	{	
		hra$r=0
		hra$K=0	
		hra$loss=0	
	}

	hra$CNmin=0
	hra$CNmax=0
	hra$group=paste(hra$Genus,hra$Num,sep="_")
	
hrm=unique(data.frame(Num=hra$Num,group=hra$group,type=hra$type,biomass=hra$biomass,bodymass=hra$Bodymass,ax=hra$ax,y=hra$y,CN=hra$CN,loss=hra$loss,r=hra$r,K=hra$K,CNmin=hra$CNmin,CNmax=hra$CNmax))
	for (i in 1:ncol(hrm))
	{
		hrm[,i]=as.character(hrm[,i])
	}
	
	#Producer
	
	if(Prod==0)
	{
		p1=c(6,"Plantae1","Producer",1,0,0,0,30,0.5,25.6,2,0,0)
		p2=c(6,"Plantae2","Producer",1,0,0,0,20,0.4,51.2,12,0,0)
		p3=c(6,"Plantae3","Producer",1,0,0,0,15,0.2,76.8,30,0,0)	
		hrm=data.frame(rbind(hrm,p1,p2,p3))	
	}

	#Microbes

	if(Mic==0)
	{
		b1=c(2,"Bacteria1","Microbes",1,0,0,0,3,0.05,46,62.5,0,0)
		b2=c(2,"Bacteria2","Microbes",1,0,0,0,6,0.05,64,150,0,0)
		b3=c(2,"Bacteria3","Microbes",1,0,0,0,10,0.05,32,62.5,0,0)	
		
		f1=c(4,"Fungi1","Microbes",1,0,0,0,5,0.01,14,12.6,0,0)	
		f2=c(4,"Fungi3","Microbes",1,0,0,0,20,0.01,46,100,0,0)
		f3=c(4,"Fungi2","Microbes",1,0,0,0,10,0.01,43,150,0,0)	
		hrm=data.frame(rbind(hrm,b1,b2,b3,f1,f2,f3))
	}	

	
	hrm=data.frame(hrm)
	hrm$bodymass=round(as.numeric(hrm$bodymass), digits = 10)
	
	param=unique(hrm)
		
	d=nrow(param)
	param[d+1,]=c(3,"Det10","Detritus",1,0,0,0,10,0.01,0,0,1,10)
	param[d+2,]=c(3,"Det20","Detritus",1,0,0,0,20,0.01,0,0,10,20)
	param[d+3,]=c(3,"Det35","Detritus",1,0,0,0,35,0.01,0,0,20,35)
	param[d+4,]=c(3,"Det50","Detritus",1,0,0,0,50,0.01,0,0,35,50)
	param[d+5,]=c(3,"DetR","Detritus",1,0,0,0,0,0.1,0,0,50,0)
	param[d+6,]=c(0,"N","Nitrogen",10,0,0,0,0,0.5,1,0,0,0)
	

	param2=param
	param2$type <- ordered(param2$type, levels=c("Nitrogen","Consumer","Producer","Detritus","Microbes"))
	param2=arrange(param2,type)
	param2$bodymass=as.double(param2$bodymass)
	options(scipen=500)
	param2$bodymass=as.character(param2$bodymass)		
	tparam=t(param2)
		
	write.table(tparam,file=paste("name_","param",".txt",sep=""),sep=" ",row.names=TRUE,col.names=FALSE, quote = FALSE)
		
######## Pref
	
	if(setPref==TRUE)
	{	
		inter=subset(inter,inter!=0)
		GpName1=merge(inter,param2,by.x="cons",by.y="Num")
		GpName1=merge(GpName1,param2,by.x="ress",by.y="Num")
		GpName=GpName1[,-(c(3,6:15,18:27))]
		GpName=subset(GpName,group.y!="DetR")
		Cdet=data.frame(rbind(c(3,"Det10",4),c(3,"Det20",3),c(3,"Det35",2),c(3,"Det50",1))) 
		colnames(Cdet)=c("Num","Sp","p")
		
		#Set preferences for basal species
		
		P=subset(param2,type=="Producer")	
	  P=P[order(P$CN, decreasing = TRUE), ]
		P$Coef=seq(2,4,by = ((4 - 2)/(nrow(P) - 1)))
		Cprod=data.frame(Num=P$Num,Sp=P$group,p=P$Coef)

		Det=c("DetR","Det50","Det35","Det20","Det10")
		PrefF=c(0.5,1,2,3,0)	
		PrefB=c(0,1,2,3,4)		
			
		if(Mic==0)
		{
			Cmic=data.frame(rbind(c(4,"Fungi1",1),c(4,"Fungi2",1),c(4,"Fungi3",1),c(2,"Bacteria1",6),c(2,"Bacteria2",6),c(2,"Bacteria3",6))) 
			colnames(Cmic)=c("Num","Sp","p")
		
			Bact1=data.frame(Nresso=Det,resso=rep(3,5),conso=rep(2,5),Nconso=rep("Bacteria1",5),p=PrefB)
			Bact2=data.frame(Nresso=Det,resso=rep(3,5),conso=rep(2,5),Nconso=rep("Bacteria2",5),p=PrefB)
			Bact3=data.frame(Nresso=Det,resso=rep(3,5),conso=rep(2,5),Nconso=rep("Bacteria3",5),p=PrefB)
			Fun1=data.frame(Nresso=Det,resso=rep(3,5),conso=rep(4,5),Nconso=rep("Fungi1",5),p=PrefF)
			Fun2=data.frame(Nresso=Det,resso=rep(3,5),conso=rep(4,5),Nconso=rep("Fungi2",5),p=PrefF)
			Fun3=data.frame(Nresso=Det,resso=rep(3,5),conso=rep(4,5),Nconso=rep("Fungi3",5),p=PrefF)
			FB=rbind(Bact1,Bact2,Bact3,Fun1,Fun2,Fun3)
		} else
		{
			M=subset(param2,type=="Microbes")	
			Kdm=descSp[,c(1,7)]
			M=merge(M,Kdm,by="Num")
			M$Coef[which(M$Kingdom=="Bacteria")]=6
			M$Coef[which(M$Kingdom=="Fungi")]=1
			Cmic=data.frame(Num=M$Num,Sp=M$group,p=M$Coef)
			FB=matrix(NA,nrow(M),5)
			for (i in 1:nrow(M))
			{
				if(M$Kingdom=="Bacteria")
				{
					FB[i,]=c(Nresso=Det,resso=rep(3,5),conso=rep(M$Num[i],5),Nconso=rep(M$group,5),p=PrefB)
				}
				else if (M$Kingdom=="Fungi")
				{
					FB[i,]=c(Nresso=Det,resso=rep(3,5),conso=rep(M$Num[i],5),Nconso=rep(M$group,5),p=PrefF)
				}
								
			}
		}
	
	
		MPD=rbind(Cmic,Cprod,Cdet)
		FBP=merge(GpName,MPD,by.x=c("ress","group.y"), by.y=c("Num","Sp"),all.x=TRUE)
		FBP=FBP[,c(2,1,3,4,7)]
		FBP$p=as.character(FBP$p)
		colnames(FBP)=colnames(FB)
		FBP=rbind(FBP,FB)
		FBP$p[which(is.na(FBP$p))]=6
		FBP=unique(FBP)
		FBP$p=as.numeric(FBP$p)

		totprey=aggregate(FBP$p,list(FBP$Nconso),sum)
		FBPrey=merge(FBP,totprey,by.x="Nconso",by.y="Group.1")
		FBPrey$inter=FBPrey$p/FBPrey$x
		FBPrey=FBPrey[,c(2,1,7)]
	}	
		
		
	colnames(FBPrey)=c("Ress","Cons","Preference")
	FBPrey=unique(FBPrey)
	tAllGp1=t(FBPrey)
	write.table(tAllGp1,file=paste("name_","pref",".txt",sep=""),sep=" ",row.names=TRUE,col.names=FALSE, quote = FALSE)

}