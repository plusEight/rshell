#testing basic cases of rshell.
ls
echo hello!
echo hello && pwd
echo hello || clear
ls -R && echo we reached this && echo whoohoo!
echo
true && free
false&&free
false || echo it is false!
#testing spacing with connectors
echo hello|| clear
ls&&hello
#further testing connectors and exec(now execv instead of execvp)
askfkjadhf || echo OR connector succeeded!
asdfasd || x=5 && echo $x
ls nonexistant || echo did not print!

#trying strange syntax to break the shell
df&&df&&df&&df&&df&&df&&df||exit
&& hello
|hello
  &&  ||  

pwd; echo; ls || clear
;
echo will this break &| echo well?
||&&;
pwd;;echo double semicolons!

#trying very long strings (starting with empty spaces)
                                                                                                                
#spaces at beginning and end
                          ls                         
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#testing odd spacing inbetween connectors and commands
ls                                     -R
echo    hello &&                           ls; echo we reach the end?
mkdir newfolder && rm newfolder
touch newfile && ls
echo I've had enough && exit
