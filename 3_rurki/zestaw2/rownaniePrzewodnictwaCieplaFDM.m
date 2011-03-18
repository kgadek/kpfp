function [jawne,niejawne,dokladne] = rownaniePrzewodnictwaCieplaFDM(a,alpha,beta,f,g,fi,n,l,tn,method)
% Oznaczenia jak w tre�ci zadania. Przyj�to, �e f(x), g(x) i fi(x) s�
% funkcjami sta�ymi.
    h = (beta-alpha)/n;
    
    % WZ�R JAWNY ==========================================================
    jawne = zeros(n-1,tn);
	jawne(:,1) = fi;
	jawne(1,:) = f;
	jawne(n-1,:) = g;
    niejawne = jawne;
	
	jA = a*a*l / (h*h);                 % Uwaga: musi zaj�� l/(h*h) < 0.25
	jB = (h*h - 2*a*a*l) / (h*h);
	% Obliczenie wynik�w
	for j=1:tn
		for i=2:n-2
			jawne(i,j+1) = jA * (jawne(i+1,j) + jawne(i-1,j)) + jB * jawne(i,j);
		end;
	end;

    % WZ�R NIEJAWNY =======================================================
    nA = -a*a*l/(h*h);
    nB = (2*a*a*l + h*h)/(h*h);
    wA = diag(nB*ones(n-1,1)) + diag(nA*ones(n-2,1),1) + diag(nA*ones(n-2,1),-1);
    % Obliczanie wynik�w
    wB = zeros(n-1,1);
    for j=2:tn+1
        wB(2:n-2) = niejawne(2:n-2,j-1);
        wB(1) = wB(1) - nA*niejawne(1,j-1);
        wB(n-3) = wB(n-3) - nA*niejawne(n-1,j-1);
        switch method
            case 1
                tmp = inv(wA)*wB;
            case 2
                tmp = wA\wB;
            otherwise
                tmp = eliminacjaGaussa(horzcat(wA,wB),n-1,n);
        end;
        niejawne(2:n-2,j) = tmp(2:n-2);
    end;
    
    % ROZWI�ZANIE DOK�ADNE ================================================
    dA = -pi*pi*l;
    dB = pi*h;
    dC = 2/pi;
    dokladne = zeros(n-1,tn+1);
  	dokladne(:,1) = fi;
	dokladne(1,:) = f;
	dokladne(n-1,:) = g;
    % Obliczenie wynik�w
    infin = 1000;       % Precyzja oblicze�
    for j=2:tn+1
        for i=2:n-2
            for s=1:infin
                dokladne(i,j) = dokladne(i,j)+dC*(1-(-1)^s)*exp(dA*s^2*j)*sin(dB*s*(i-1))/s;
            end;
        end;
    end;
end
