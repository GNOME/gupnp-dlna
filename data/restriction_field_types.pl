#!/usr/bin/perl

use strict;
use warnings;
use XML::Parser::Expat;

my %restrictions = ();
my @restriction_stack = [];

sub start
{
    my ($p, $el, %atts) = @_;

    if ($el eq 'restriction')
    {
	if (exists ($atts{'type'}))
	{
	    my $type = $atts{'type'};

	    unless (exists ($restrictions{$type}))
	    {
		$restrictions{$type} = {};
	    }
	    push (@restriction_stack, $type);
	}
    }
    elsif ($el eq 'field')
    {
	if (exists ($atts{'name'}))
	{
	    my $name = $atts{'name'};

	    if (exists ($atts{'type'}))
	    {
		my $type = $atts{'type'};

		if (@restriction_stack)
		{
		    my $restriction_type = $restriction_stack[-1];
		    my $fields = $restrictions{$restriction_type};

		    if (exists $fields->{$name})
		    {
			if ($fields->{$name} ne $type)
			{
			    print STDERR "Field '$name' of '$restriction_type' has different types: $fields->{$name} and $type.\n";
			}
		    }
		    else
		    {
			$fields->{$name} = $type;
		    }
		}
		else
		{
		    print STDERR "Field '$name' of type '$type' is not a part of restriction.\n";
		}
	    }
	    else
	    {
		print STDERR "Field '$name' has no type attribute.\n";
	    }
	}
	else
	{
	    print STDERR "Unnamed field.\n";
	}
    }
}

sub end
{
    my ($p, $el) = @_;

    if ($el eq 'restriction')
    {
	if (@restriction_stack)
	{
	    pop (@restriction_stack);
	}
	else
	{
	    print STDERR "Tried to pop a restriction but stack is empty.\n";
	}
    }
}

foreach my $file (@ARGV)
{
    my $parser = XML::Parser::Expat->new ();

    $parser->setHandlers ('Start' => \&start,
			  'End' => \&end);

    $parser->parsefile ($file);
    $parser->release ();
}

foreach my $type (sort (keys (%restrictions)))
{
    my $fields = $restrictions{$type};

    print "Restriction type: $type\n";
    foreach my $field (sort (keys (%{$fields})))
    {
	my $field_type = $fields->{$field};

	print "  $field [$field_type]\n";
    }
}
