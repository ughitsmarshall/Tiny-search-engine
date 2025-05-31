# Tiny Search Engine Querier
## Created by Marshall Carey-Matthews

## Structure

The structure of this querier follows the requirements, design, and implementation in this repository. The only known limitation is that the query buffer cutoff is 1000 characters, meaning queries longer than 1000 characters will be truncated. This causes the risk for a query word that does not meet the character requirement. 

## Assumptions

The code assumes that the user is using a UNIX system where the directory dividers are "/" and not "\". The user may or may not enter the "/" in directory arguments, if they so choose.
