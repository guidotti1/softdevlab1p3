// JavaScript for Name Data Lookup Demo
// Jim Skon, Kenyon College, 2019
var searchType;  // Save search type here
var charNumber;
var data;
var xhr = new XMLHttpRequest();

$(document).ready(function () {
    console.log("start!");
    alert("Welcome to Marvel character lookup! Make sure to click clear page after going through a search");
    searchType="Temp";
    charNumber ="0";
    data="Temp";
    $("#search-btn").click(getMatches);

    $("#clear").click(clearResults);

    $(".dropdown-menu li a").click(function(){
	console.log("pick!"+$(this).text());
	$(this).parents(".btn-group").find('.selection').text($(this).text());
	searchType=$(this).text();
    });
    
});

// Build output table from comma delimited list
function nameTable(list) {
    var result = '<table class="w3-table-all w3-hoverable" border="2"><tr><th>Number</th><th>Name</th>';
    var a = list.split(",");
    var aLen = a.length;
    for (var i = 0; i < aLen-1; i+=2) {
	result += "<tr><td>"+a[i]+"</td><td>"+a[i+1]+"</td><tr>";
    }
    result += "</table>";
	userSelectCharacter();
    return result;
}

function userSelectCharacter()
{
	var result = '<form>';
	result += '	<div class="form-group">';
	result += '	<label for="charNumber">Enter Desired Character Number</label>';
	result += '		<input type="number" id ="charNumberInput" class="form-control" placeholder="Enter Desired Character Number">';
	result += ' <\div>';
	var button  ='<button type="button" id ="charNumberButton" class="btn btn-default">Submit</button>';
	var $button = $(button).bind("click", function()
		{
		getCharacter();
		});	
   	var close = '</form>';
   	$('#characterNumberDisplay').append(result);
    	$('#characterNumberDisplay').append($button);
    	$('#characterNumberDisplay').append(close);
}


function processResults(results) 
{
    $('#searchresults').empty();
    $('#searchresults').append(nameTable(results));
}

function clearResults() 
{
    console.log("results being cleared");
    $('#searchresults').empty();
    $('#characterNumberDisplay').empty();
    searchType="Temp";
    charNumber ="0";
    data="Temp";
    $(".dropdown-menu li a")[0].click();
    //window.location.reload();
 }

function getMatches()
{
    //console.log("in get matches");
    data = $('#search').val();
    if ($('#search').val().length < 2) return;
    $('#searchresults').empty();
    $.ajax({
		url: '/cgi-bin/guidotti1_marvelLookup.cgi?name='+data+'&type_select='+searchType+"&charNumber="+charNumber,
		dataType: 'text',
		success: processResults,
		error: function(){alert("Error: Something went wrong");}
    });
}

function getCharacter()
{
    //console.log("in get character!!!");
    charNumber=$('#charNumberInput').val();
    $.ajax({
		url: '/cgi-bin/guidotti1_marvelLookup.cgi?name='+data+'&type_select='+searchType+"&charNumber="+charNumber,
		dataType: 'text',
		success: processCharacter,
		error: function(){alert("Error: Something went wrong");}
    });
}

function clearCharacter()
{
	 $('#characterNumberDisplay').empty();
}

function processCharacter(result)
{ 
	var attributeList = '<ul class="list-group">';
	var a = result.split(",");
	var aLen = a.length;
    	for (var i = 0; i < aLen; i++) 
    	{
		attributeList += '<li class="list-group-item">'+a[i]+'</li>';
    	}
    	attributeList += '</ul>'
    	$('#characterNumberDisplay').append(attributeList);
}	

