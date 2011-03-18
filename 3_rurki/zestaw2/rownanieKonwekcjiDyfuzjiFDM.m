function solution = rownanieKonwekcjiDyfuzjiFDM(a,b,c,n,u0,u1,method)
% Zastosowanie MRS do rozwi¹zania równania konwekcji-dyfuzji.

    %  method = 1 - rozwi¹zanie poprzez u¿ycie macierzy odwrotnej
    %           2 - rozwi¹zanie poprzez u¿ycie metody eliminacji Gaussa
    %           3 - w³asna implementacja eliminacji Gaussa
    
    % Kilka przydatnych zmiennych
    h = 1/n;
    h2 = 1/(n*n);
    
    % Generuj macierz B
    B = arrayfun(@(i)max(a*i/n+b),transpose(1:n-1));
    B(1,1) = B(1,1) + u0*(c/(2*h)+1/h2);
    B(n-1,1) = B(n-1,1) - u1*(c/(2*h) - 1/h2);
    
    % Generuj macierz A
    A = diag( (2/h^2) * ones(n-1,1)) + diag( (c/(2*h)-1/h2)*ones(n-2,1),1) + diag( (-c/(2*h)-1/h2)*ones(n-2,1),-1);
    
    % Rozwi¹zanie przybli¿one
    switch method
        case 1
            solution = inv(A)*B;
        case 2
            solution = A\B;
        otherwise
            solution = eliminacjaGaussa(horzcat(A,B),n-1,n);
    end;
end

