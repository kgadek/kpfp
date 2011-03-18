% RÓWNANIE KONWEKCJI-DYFUZJI ==============================================

% Badanie b³êdów
for n=[5 10 50 100 500]
    errMax = 0;
    errMin = 1000000000000000;    
    errs = zeros(n-1,1);
    for p=[2 2 2 5; 5 5 5 9; 2 2 2 5; 0 0 1 10; 0 1 1 20]
        a = p(1); b=p(2); c=p(3); u0=p(4); u1=p(5);
        k = (2*c*c*(u1-u0)-a*c-2*a-2*b*c)/(2*c*(exp(c)-1));
        exact = transpose(arrayfun(@(i)max(k*(exp(c*i/n)-1)/c+a*i*i/(2*c*n*n)+(a/(c*c)+b/c)*i/n+u0),1:n-1));
        tmp = rownanieKonwekcjiDyfuzjiFDM(a,b,c,n,u0,u1,2);
        tmp = abs(tmp-exact);
        plot(tmp)
        if max(tmp) > errMax
            errMax = max(abs(tmp-exact));
        end;
        if min(tmp) < errMin
            errMin = min(tmp);
        end;
        errs = errs + tmp;
    end;
    errMax
    errMin
    errAvg = mean(errs)/3
end;


% Badanie czasu wykonywania
for n=[5 10 50 100 500]
    tSum = [0 0 0];
    for meth=1:3
        for p=[2 2 2 5; 5 5 5 9; 2 2 2 5; 0 0 1 10; 0 1 1 20]
            a = p(1); b=p(2); c=p(3); u0=p(4); u1=p(5);
            tic;
            for i=1:10
                tmp = rownanieKonwekcjiDyfuzjiFDM(a,b,c,n,u0,u1,meth);
            end;
            tSum(meth) = tSum(meth) + toc;
        end;        
    end;
    tSum = tSum / 30
end;

[a,b,c] = rownaniePrzewodnictwaCieplaFDM(1,0,1,0,0,1,50,0.0001,100,2);