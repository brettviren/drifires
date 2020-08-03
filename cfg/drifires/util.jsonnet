{

    // Return array of objects keys
    keys(obj) :: std.objectFields(obj),

    // Return array of objects values
    values(obj) :: std.map(function(x) obj[x], std.objectFields(obj)),

}

