### Syntax
Basic Syntax: `${<varname>}`  
Advanced syntax: `${<varname>}(<parm>, <parm>, <parm>....)`  

### Guidelines
Variable name:
- Must be alphanumeric, without special characters
- Allowed special characters:
    - `.` (Point)
    - `_` (?????)
Paramethers/Values
- Could be every character
- Could be in `"` (No escaping required, exept the `"`)
- Escape character is `\`

### Examples
- Example 1:  
    Input: `Hello ${world}`  
    Output: `Hello Mars`  
    Variables:  
    - `world` = `Mars`  
  
- Example 2:  
    Input: `Hello ${world}`  
    Output: `Hello Mars`  
    Variables:  
    - `world` = `${world.type}`  
    - `world.type` = `Mars`  
  
- Example 3:  
    Input: `Hello ${world}(sun)`  
    Output: "Hello sun"  
    Variables:
    - `world` = function (vars) => "${world.type.$vars[0]}"  
    - "world.type.sun" = "Sun"  