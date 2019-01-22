#!/bin/bash

latex timetable.tex
latex timetable.tex
dvips timetable.dvi -o timetable.ps
ps2pdf timetable.ps
