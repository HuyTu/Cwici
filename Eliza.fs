//
// Eliza.fs
// The Eliza program 
// processes the input of a user and responds in a manner that mimics a Rogerian therapist. At that 
// time people were amazed that the computer seemed so human-like, but as we will see the 
// 'intelligence' was based on simple pattern matching. 
//
// @author: Huy Tu
// @date: 11/17/2014
//
module Eliza
    open System
    open System.Text.RegularExpressions
    
    //
    // Our ElizaMatch is implemented as an active pattern 
    // that has the match construct value, which 
    // is the user’s input sentence, and one additional argument 
    // that is the regular expression representing a particular pattern. 
    //          
    let (|ElizaMatch|_|) (pattern:string) inputSentence =
        let result = Regex.Match(inputSentence, pattern)
        if result.Success
            then Some (List.tail [ for g in result.Groups -> g.Value ])
        else None
    
    let data = ["Go on"; "In what way"]
    let random = new System.Random(31);;
    
    //
    // The matchInput function uses our ElizaMatch active pattern. 
    // The string parameter sentence is what the user typed into the computer. 
    // The alternative patterns are checked in turn; 
    // if there is a match, in other words the active pattern returned Some, 
    // then the result returned is used to formulate and print the response.   
    //          
    let matchInput (sentence:string) = 
        let response = 
            match sentence with 
            | ElizaMatch "^.*\s+mother\s+.*$" elems ->
                "Tell me about your family"
            | ElizaMatch "^.*my\s+(\w+)\s+.*me.*$" elems -> 
                "Tell me about your " + (List.nth elems 0).ToString()
            | ElizaMatch "^.*i\s+am\s+(.*)$" elems -> 
                "I am sorry to hear you are " + 
                (List.nth elems 0).ToString()
            | ElizaMatch "^.*am\s+i\s+(.*)$" elems -> 
                "Do you believe you are " +
                (List.nth elems 0).ToString()
            | ElizaMatch "^.*you\s+(.*)\s+me.*$" elems -> 
                "Why do you think I " + 
                (List.nth elems 0).ToString() + 
                " you"
            | ElizaMatch "^.*can\s+I\s+(.*)\s+you$" elems ->
                "Yes, you can " +
                (List.nth elems 0).ToString() + " me" 
            | ElizaMatch "^.*why\s+(.*)$" elems ->
                "Why are you asking me this"
            | ElizaMatch "^.*I\s+don't\s+know\s+(.*)$" elems ->
                "How come you don't know about " +
                (List.nth elems 0).ToString() 
            | ElizaMatch "^.*I\s+can't\s+(.*)$" elems ->
                "Do you want to be able to " +
                (List.nth elems 0).ToString()
            | ElizaMatch "^.*i\s+(.*)my\s+(.*)$" elems -> 
                "Why do you " + 
                (List.nth elems 0).ToString() + 
                " your " +
                (List.nth elems 1).ToString()
            |"yes" -> "Please continue."
            |"no" -> "Please continue"
            | _ -> List.nth data (random.Next(0,2))
        printfn "%A\n" response
    
    //
    // The eliza function itself is a simple tail-recursive function 
    // that reads input from the user and generates a response based on that input. 
    // If the input is the empty string, the program is exited.   
    //    
    let rec eliza() =
        let sentence = Console.ReadLine()
        if sentence <> "" then 
            matchInput sentence
            eliza()
        else
            printfn "Goodbye."
            System.Environment.Exit;
    
    let test = eliza();;
