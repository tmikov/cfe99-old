set ext=-x *.obj -x *.exe -x *.ilk -x *.zip -x *.out -x *.pch -x *.pdb -x *.idb -x *.ncb

zip -r -u -9 cnew.zip * %ext%

copy cnew.zip \backup /r
