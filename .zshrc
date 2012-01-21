# Lines configured by zsh-newuser-install
HISTFILE=~/.histfile
PYTHONSTARTUP=~/.rcpython.py
HISTSIZE=300000
SAVEHIST=300000
export PATH=$PATH:~/.local/bin
export EDITOR="vim"
bindkey -v
# End of lines configured by zsh-newuser-install
# The following lines were added by compinstall
#nastepna linie odkomentuj i zmien na swoj katalog domowy
#zstyle :compinstall filename '/home/dex/.zshrc'
setopt autocd
setopt auto_param_slash
setopt append_history
setopt menu_complete
setopt completeinword
setopt no_alwaystoend

autoload -Uz compinit
compinit
# End of lines added by compinstall
alias gitg='gitg --all 2&>/dev/null'
alias 7zau='7za a -t7z -m0=lzma -mx=9 -mfb=64 -md=32m -ms=on'
alias studentindexclean='find . -regex ".*index\.html\?C=[SMDN];O=[AD]" -exec rm {} \;'
alias icanhazpid='pidof'
alias :q='exit' #fajny alias do wychodzenia z shella
alias ls='ls -h --color=auto'
alias ocaml='rlwrap ocaml'
export PS1="[$(print '%{\e[0;34m%}%n%{\e[0;35m%}@%{\e[0;33m%}%M %U%{\e[1;31m%}%~%u%{\e[0m%}]%{\e[1;36m%}%#%{\e[0m%} ')" #kolorki - powinny dzialac na dowolnym porzadnym emulatorze terminala
export PS2="$(print '=========%U%{\e[1;31m%}%~%u%{\e[0m%}=%{\e[1;36m%}%#%{\e[0m%} ')"
function zle-line-init zle-keymap-select {
	RPS1="[$(print '%{\e[0;36m%}%*%{\e[0;30m%}] ')${${KEYMAP/vicmd/-- CMD --}/(main|viins)/-- INS --}" #to pokazuje, czy w vi mode jestes w INS, czy w CMD
	RPS2=$RPS1
	RPS3=$RPS1
	RPS4=$RPS1
	zle reset-prompt
}
zle -N zle-line-init
zle -N zle-keymap-select

bindkey "OH"  beginning-of-line
bindkey "OF" end-of-line
bindkey "[5~" beginning-of-history
bindkey "[6~" end-of-history
bindkey "[3~" delete-char
bindkey "[2~" quoted-insert
bindkey "" vi-up-line-or-history
bindkey "" vi-down-line-or-history
bindkey "[1;5D" vi-backward-blank-word
#bindkey "OF" vi-forward-blank-word
bindkey "[1;5C" vi-forward-blank-word
bindkey "Od" vi-backward-blank-word
bindkey "Oc" vi-forward-blank-word
bindkey "[7~" beginning-of-line
bindkey "[8~" end-of-line

# allow approximate
zstyle ':completion:*' format '%{[32m%}-=> %{[01m%}%d%{[0m%}'
#zstyle ':completion:*' completer _complete _match _approximate
zstyle ':completion:*' completer _expand _complete _ignored _approximate _prefix _expand _complete
zstyle ':completion:*:match:*' original only
#zstyle ':completion:*:approximate:*' max-errors 2 numeric
zstyle ':completion:*:approximate:*' max-errors 1 numeric

zstyle ':completion:*:*:kill:*' menu yes select #wyswietla liste przy tabie killa, wczesniejsze wyswietlaja liste argumentow dla niektorych komend!
zstyle ':completion:*:kill:*' force-list always

zstyle ':completion:*:processes' list-colors '=(#b)(?????)(#B)?????????????????????????????????([^ ]#/)#(#b)([^ /]#)*=00=01;31=01;33'

LS_COLORS='rs=0:di=01;34:ln=01;36:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:su=30;41:sg=30;43:tw=30;42:ow=34;42:st=30;44:ex=01;32:';
export LS_COLORS #kolory listy przy wcisnieciu tab

zstyle ':completion:*:default' list-colors ${(s.:.)LS_COLORS}

setopt extended_glob
bindkey '^[[Z' reverse-menu-complete
#z vi mode mozna korzystac wciskajac ESC albo C-[, jak zwykle
#wpisywanie komend zawsze zaczyna w insert mode, wiec nie trzeba za kazdym razem dawac i przed rozpoczeciem pisania :)

export PERL_LOCAL_LIB_ROOT="/home/konrad/perl5";
export PERL_MB_OPT="--install_base /home/konrad/perl5";
export PERL_MM_OPT="INSTALL_BASE=/home/konrad/perl5";
export PERL5LIB="/home/konrad/perl5/lib/perl5/i686-linux-thread-multi:/home/konrad/perl5/lib/perl5";
export PATH="/home/konrad/perl5/bin:$PATH";
