function solution = eliminacjaGaussa(A,m,n)
        solution = zeros(m,1);
        i = 1;      % _____ wyznaczenie koñcowego rozwi¹zania
        j = 1;
        while ((i<=m) && (j<=n))
            maxi = i;           % pivotowanie
            for k=i+1:m
                if abs(A(k,j)) > abs(A(maxi,j))
                    maxi = k;
                end;
            end;
            if A(maxi,j) ~= 0
                tmp = A(i,:);   % zamieñ wiersze i oraz maxi
                A(i,:) = A(maxi,:);
                A(maxi,:) = tmp;
                A(i,:) = A(i,:) / A(i,j);   % normalizacja do jedynki
                for u=i+1:m
                    A(u,:) = A(u,:)-A(u,j)*A(i,:);
                end;
                i = i+1;
            end;
            j = j+1;
        end;
        solution(m,1) = A(m,n);
        for i=1:m-1 % _____ wyznaczenie koñcowego rozwi¹zania
            solution(m-i,1) = A(m-i,n)-A(m-i,m-i+1)*solution(m-i+1,1);
        end;
end
