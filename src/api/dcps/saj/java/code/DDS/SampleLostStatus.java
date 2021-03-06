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

package DDS;


public final class SampleLostStatus 
{
  public int total_count = (int)0;
  public int total_count_change = (int)0;

  public SampleLostStatus ()
  {
  } // ctor

  public SampleLostStatus (int _total_count, int _total_count_change)
  {
    total_count = _total_count;
    total_count_change = _total_count_change;
  } // ctor

} // class SampleLostStatus
