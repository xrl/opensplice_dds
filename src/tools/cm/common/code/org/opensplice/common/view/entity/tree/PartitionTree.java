/*
 *                         OpenSplice DDS
 *
 *   This software and documentation are Copyright 2006 to 2010 PrismTech
 *   Limited and its licensees. All rights reserved. See file:
 *
 *                     $OSPL_HOME/LICENSE 
 *
 *   for full copyright notice and license terms. 
 *
 */
package org.opensplice.common.view.entity.tree;

import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeSelectionModel;

import org.opensplice.cm.Participant;
import org.opensplice.common.CommonException;
import org.opensplice.common.controller.EntityTreeWillExpandListener;
import org.opensplice.common.model.ModelRegister;

/**
 * Concrete implementation of an EntityTree. It represents a tree which holds
 * all Partition entities in the connected kernel and shows its dependant 
 * entities as children.
 *  
 * @date Oct 5, 2004
 */
public class PartitionTree extends EntityTree {

    /**
     * Constructs a new PartitionTree.
     * 
     * @param _participant The participant where to resolve the domains from.
     * @param _register The ModelRegister that needs to be triggered on changes
     *                  in the tree. It may be null when no component needs
     *                  to be triggered.
     * @throws CommonException Thrown when the supplied Participant is not valid.
     */
    public PartitionTree(Participant _participant, ModelRegister _register, boolean childrenVisible) throws CommonException {
        super(_participant, _register, childrenVisible);
        rootNode = new PartitionTreeNode(participant, this);
        this.setModel(new DefaultTreeModel(rootNode));
        this.setEditable(false);
        this.getSelectionModel().setSelectionMode
            (TreeSelectionModel.SINGLE_TREE_SELECTION);
        willExpandListener = new EntityTreeWillExpandListener(this);
        this.addTreeWillExpandListener(willExpandListener);
        rootNode.refresh();
    }

}
