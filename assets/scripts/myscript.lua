
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
    return result
end