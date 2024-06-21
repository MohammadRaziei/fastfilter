classdef TestMex < matlab.unittest.TestCase
    methods(Test)

        function test_arrayProduct(tc)
            a = eye(3);
            try
                x = arrayProduct(2, a);
            catch excp
                assumer(tc, excp)
            end
            
            tc.verifyEqual(x, 2*a)
        end
    end
    
end
