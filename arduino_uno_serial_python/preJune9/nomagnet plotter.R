df_files <- file.info(Sys.glob("C:\\Users\\451516\\Documents\\github\\Arduino\\arduino_uno_serial_python\\*.csv"))
lfile=row.names(df_files)[which.max(df_files[["ctime"]])]
#lfile="C:\\Users\\451516\\Documents\\github\\Arduino\\arduino_uno_serial_python\\log_data_magnet_RevedOLDSETUPwithBuck_6mmMOVE_NEWCOIL_5700mf1_20220515-181602.csv"

library(plotly)
library(DT)
library(ggplot2)
require(tidyr); 
library(scales)
library(dplyr)
library(plyr)
library(rootSolve)
library(npreg)
library(numDeriv)
#install.packages('numDeriv')
processedpath="C:\\Users\\451516\\Documents\\github\\Arduino\\arduino_uno_serial_python\\processedData\\"
df_files <- file.info(Sys.glob("C:\\Users\\451516\\Documents\\github\\Arduino\\arduino_uno_serial_python\\CalibrationData\\*.csv"))
cfile=row.names(df_files)[which.max(df_files[["ctime"]])]
#lfile="C:\\Users\\451516\\Documents\\github\\Arduino\\arduino_uno_serial_python\\log_data_magnet_yes_MAGNET0_20220517-155650.csv"
df=read.csv(lfile)
#cfile="C:\\Users\\451516\\Documents\\github\\Arduino\\arduino_uno_serial_python\\CalibrationData\\log_Calib_data_dayLight_20220515-175632.csv"
doMpos<-FALSE
calibdata=read.csv(cfile)


df$sourcefile<-basename(lfile)
df$voltage_V<-df$voltage/1024*5*100
#df$CE_V<-df$ CEValue/1024*5*100*3
#df$CE_V<-df$CEValue/255*2.5
df$current_A<-(df$current-512)/36


if("trigger" %in% names(df)){
  #df$sourcepath<-lfile
  df$triggered<-1
  df$triggered[which(df$trigger<10)]<-0
  
  dts<- as.data.frame(df %>% dplyr::group_by(triggered,sourcefile)%>%dplyr::summarise(mxT = max(time), CapInitV = max(voltage_V)))
  dts<-dts[dts$triggered==1,]
  dts$triggered<-NULL
  
  df<-merge(x = df, y = dts, by="sourcefile", all.x = TRUE)
  df$time_ms<-df$time-df$mxT
}else{
  
  
  dts<- as.data.frame(df %>% dplyr::group_by(sourcefile)%>%dplyr::summarise(mnT = min(time), CapInitV = max(voltage_V)))
  
  
  df<-merge(x = df, y = dts, by="sourcefile", all.x = TRUE)
  
  df$time_ms<-(df$time-df$mnT)/1000
  
  
}


df<-df[df$time_ms>-1,]



df$voltage_n<-df$voltage_V/df$CapInitV
df$current_n<-df$current_A/max(df$current_A)
vmax=round(max(df$voltage_V),2)
imax=round(max(df$current_A),2)



##smoothz<-smooth.spline(df$time_ms,df$zpos)
tsplit=120


mLc_hoscV = df$Inductance[1]
mRc_hoscV = df$Resistance[1]
mCc_hoscV = df$Capacitor[1]
mv0_hoscV = df$CapInitV[1]
mw0_hosc_rlc = (mCc_hoscV * mLc_hoscV)^(-0.5) * 10^4.5   
mzeta_rlc = (1 / 2) * mRc_hoscV * (mCc_hoscV / mLc_hoscV)^0.5* 10^(-1.5)

rlc <- function(v0,w0,zeta,t) {
  wd=w0*abs(1-zeta^2)^0.5
  (v0* cosh(wd* t) +sinh(wd *  t) * (zeta* v0/(abs(1-zeta^2))^0.5))* exp(-zeta * w0 * t )
  
}

irlc <- function(v0,w0,zeta,t) {
  wd=w0*abs(1-zeta^2)^0.5
  v0*mCc_hoscV*10^(-6)*w0* sinh(wd *  t)/abs(1-zeta^2)^0.5 * exp(-zeta * w0 * t )
  
}



df$voltage_V_th<-rlc(mv0_hoscV,mw0_hosc_rlc,mzeta_rlc,df$time_ms/1000)

df$voltage_V_th_n<-df$voltage_V_th/df$CapInitV

df$current_A_th<-irlc(mv0_hoscV,mw0_hosc_rlc,mzeta_rlc,df$time_ms/1000)
df$current_A_th[is.na(df$current_A_th)]<-0;
df$current_A_th[is.infinite(df$current_A_th)]<-0;
df$current_th_n<-df$current_A_th/max(df$current_A_th)

imaxt=round(max(df$current_A_th),2)
vmaxt=round(max(df$voltage_V_th),2)
names(df)
hline <- function(y = 0, color = "black") {
  
  list(
    
    type = "line",
    
    x0 = 0,
    
    x1 = 1,
    
    xref = "paper",
    
    y0 = y,
    
    y1 = y,
    
    line = list(color = color)
    
  )
  
}

vline <- function(x = 0, color = "green") {
  
  list(
    
    type = "line",
    
    y0 = 0,
    
    y1 = 1,
    
    yref = "paper",
    
    x0 = x,
    
    x1 = x,
    
    line = list(color = color, dash="dot")
    
  )
  
}

ay <- list(
  
  tickfont = list(color = "red"),
  
  overlaying = "y",
  
  side = "right",
  
  title = "<b>Normalized Voltage and Current</b> ")

fig <- plot_ly()%>% 
  add_trace( x = df$time_ms, y =  df$voltage_V, name = paste0('voltage,',vmax), fillcolor = 'rgba(255,0,127,0.5)', yaxis = "y2", mode = 'lines+markers') %>%
  add_trace( x = df$time_ms, y =  df$voltage_V_th, name = paste0('voltage_th,',vmaxt), fillcolor = 'rgba(255,0,127,0.5)', yaxis = "y2", mode = 'lines+markers') %>%
  
  
  add_trace( x = df$time_ms, y =  df$current_A, name = paste0('current,',imax), fillcolor = 'rgba(255,0,127,0.5)', mode = 'lines+markers') %>%
  add_trace( x = df$time_ms, y =  df$current_A_th, name = paste0('current th,',imaxt), fillcolor = 'rgba(255,0,127,0.5)', mode = 'lines+markers') %>%
  
  
  #  add_trace( x = dfsmooth$time_ms, y =  df$speed, name ='speed', fillcolor = 'rgba(255,0,127,0.5)', mode = 'lines') %>%
  # add_trace( x = dfsmooth$time_ms, y =  df$speed_sfit, name ='speed_sfit', fillcolor = 'rgba(255,0,127,0.5)', mode = 'lines') %>%
  


layout(
  
  title = paste('Magnet Position(mm) vs Time  ( @ Cap Volt ', round(max(df$voltage_V),1), "V, cross time:",round(crossings[1],1) ," ms)"),
  yaxis2 = ay,
  
  plot_bgcolor='#e5ecf6',  
  
  xaxis = list(  
    
    title = 'time(ms)',
    
    zerolinecolor = '#ffff',  
    
    zerolinewidth = 2,  
    
    gridcolor = 'ffff'),  
  
  yaxis = list(  
    
    title = 'Voltage',
    
    zerolinecolor = '#ffff',  
    
    zerolinewidth = 2,  
    
    gridcolor = 'ffff'),
  
  showlegend = TRUE, width = 1100)

fig




write.csv(df,paste0(processedpath,basename(lfile)), row.names = FALSE)


folderdests<-c("C:/Users/451516/Documents/github/tetraquark_c/static/EBOOKS/opticalShutter/_book/"
               ,"C:/Users/451516/Documents/github/tetraquark_c/public/EBOOKS/opticalShutter/_book/"
)


foldersource<-"C:\\Users\\451516\\Documents\\github\\Arduino\\arduino_uno_serial_python\\processedData"
for (folderdest in folderdests){
  
  dir.create(folderdest)
  file.copy(foldersource,folderdest, recursive=TRUE)
}
