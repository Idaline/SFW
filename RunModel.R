
RunModel=function(param,pref,name,vecArg)
{
	result=(paste("name_","result",".txt",sep="")
	
	system("make")
	
	system(paste("src/BB",param,pref,result,vecArg,sep=" "))

}