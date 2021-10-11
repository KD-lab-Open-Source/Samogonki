@echo off
cd ..\..\
parser iscreen.scr acs_scr.set -ifISCREEN\SCRIPTS\ -ofRESOURCE\ISCREEN\ -outiscreen.scb
parser arcane.scr acs_scr.set -ifISCREEN\SCRIPTS\ -ofRESOURCE\ISCREEN\ -outarcane.scb
parser sprites.scr acs_scr.set -ifISCREEN\SCRIPTS\ -ofRESOURCE\ISCREEN\ -outarcane.scb
