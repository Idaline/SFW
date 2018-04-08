#'Creates a dataframe species interactions
#'
#' @param path of a dataframe (ex:'param.txt') with species parameters created by 'CreateSource'
#' @pref path of a dataframe (ex:'pref.txt') with species interaction preferences created by 'CreateSource'
#' @name the name you want to give to your simulation (character)
#' @vArg a vector with your simulation parameters, in order: ending time, time step, nitrogen input, percentage nitrogen output, litter (detritus) input, C:N of the litter input.
#' @return a .txt with species biomass as a function of time
#' @export
RunModel=function(param,pref,name,vArg)
{
	result=(paste("name_","result",".txt",sep=""))
	
	vecArg=matrix(NA,6,2)	
	vecArg[,1]=c('Tf','Tp','Nin','Nout','DetIn','CNDetIn')
	vecArg[,2]=vArg

	write.table(vecArg,file='vecArg.txt',row.names=FALSE,col.names=FALSE, quote = FALSE)
	
	dir=path.package('SFW')
	system(paste(paste(dir,"/src/FW",sep=''),param,pref,result,'vecArg.txt',sep=" "))
	system('rm -rf vecArg.txt')

}