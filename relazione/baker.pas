function BakerDitchfield( this_name : name, args : list< set< tree > > ) : 
  tree_list
begin
  result_trees : set< tree > = {}
  for each interpretation (formals) corresponding to this_name
    if formals.length == args.length then
      new_tree : tree
      new_tree.type = return_type
      new_tree.cost = 0
      for i = 1 to formals.length
        if there exists j in args[i] such that j.type can be converted 
          to formals[i].type then
          find k in args[i] such that 
            conversion_cost(k.type, formals[i].type) + k.cost is minimized
          new_tree.child[i] = k
          new_tree.cost += k.cost
          new_tree.cost += conversion_cost(k.type, formals[i].type)
        else
          skip to next interpretation
          (i.e. this interpretation is not consistent with the 
          possible argument types)
	      end if
      end for
      if this interpretation is valid then
        if there exists i in result_trees such that i.type == new_tree.type 
        then
          if i.cost == new_tree.cost then
            i.ambiguous = true
          else if i.cost < new_tree.cost then
            remove i from result_trees
            add new_tree to result_trees
          (else throw away new_tree)
	        end if
        else
          add new_tree to result_trees
	      end if
      end if
    end if
  end for
  for each tree in result_trees
    if tree.ambiguous then
      remove tree from result_trees
    end if
  end if
  return result_trees
end
