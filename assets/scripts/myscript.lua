
-- This is a lua global variable
some_variable = 3^( 5 * 2 )

-- this is a lua table
config = {
    title = "My game engine",
    fullscreen = false,
    resolution = {
        width = 800,
        height = 600
    }
}

-- this is a lua function
function factorial(n)
    if n < 0 then
        return nil, "Factorial is not defined for negative numbers"
    end

    local result = 1
    for i = 2, n do
        result = result * i
    end
    print(result)
    return result
end

-- calling a function from c++
print("The cube of the number 3 is equal to "..cube(3));