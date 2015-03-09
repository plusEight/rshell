#testing basic cases of rshell.
ls
clear
echo hello!
echo hello && clear
echo hello || clear
ls -R && echo we reached this && clear
echo
#further testing connectors and exec(now execv instead of execvp)
askfkjadhf || echo OR connector succeeded!
asdfasd || x=5 && echo $x
ls nonexistant || echo did not print!

#trying strange syntax to break the shell
&& hello
|hello
  &&  ||  

pwd; echo; ls || clear
;
echo will this break &| echo well?
||&&;
clear;;echo double semicolons!

bin/rshell && echo hello && ls
exit
echo done?
exit
